//------------------------------------------------------------------------------
// modreader.cpp
//------------------------------------------------------------------------------
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301  USA
//
//------------------------------------------------------------------------------
// Copyright (C) 2019 Pol Marcet Sardà <polmarcetsarda@gmail.com>
//------------------------------------------------------------------------------

#include "modreader.h"

#include "datastreamoperatorwrapper.h"
#include "wadseeker/zip/unarchive.h"

#include <QBuffer>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>

QSharedPointer<ModReader> ModReader::create(const QString &path)
{
	QFileInfo f(path);
	if (f.suffix().compare("wad", Qt::CaseInsensitive) == 0)
	{
		return QSharedPointer<ModReader>(new WadReader(path));
	}
	else if (f.suffix().compare("zip", Qt::CaseInsensitive) == 0 ||
		f.suffix().compare("7z", Qt::CaseInsensitive) == 0)
	{
		return QSharedPointer<ModReader>(new CompressedReader(path));
	}
	else if (f.suffix().compare("pk3", Qt::CaseInsensitive) == 0 ||
		f.suffix().compare("pk7", Qt::CaseInsensitive) == 0)
	{
		return QSharedPointer<ModReader>(new PkReader(path));
	}
	return QSharedPointer<ModReader> ();
}

DClass<WadReader>
{
public:
	QString filepath;
	bool isIwad;
	QList<DirectoryEntry> directory;
};

DPointered(WadReader)

WadReader::WadReader(const QString &path)
{
	d->filepath = path;
}

WadReader::~WadReader()
{
}

bool WadReader::load()
{
	QFile f(d->filepath);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream streamUnwrapped(&f);
		streamUnwrapped.setByteOrder(QDataStream::LittleEndian);
		DataStreamOperatorWrapper stream(&streamUnwrapped);

		QString isIwadText = stream.readRaw(4);
		d->isIwad = (isIwadText == "IWAD");
		stream.skipRawData(4);

		int directoryPosition = stream.readQInt32();
		stream.skipRawData(directoryPosition - 12);
		while (stream.hasRemaining())
		{
			DirectoryEntry dirEntry;
			dirEntry.position = stream.readQInt32();
			dirEntry.size = stream.readQInt32();
			dirEntry.name = stream.readRaw(8);
			d->directory << dirEntry;
		}
	}
	else
		return false;
	return true;
}

bool WadReader::isIwad()
{
	return d->isIwad;
}

QList<DirectoryEntry> WadReader::getDirectory()
{
	return d->directory;
}

QStringList WadReader::getAllMaps()
{
	QStringList maps;
	QStringList names;
	QStringList lumpsToCheckFor = {"THINGS", "LINEDEFS", "SIDEDEFS", "VERTEXES",
		"SEGS", "SSECTORS", "NODES", "SECTORS", "REJECT", "BLOCKMAP"};

	foreach (DirectoryEntry dirEntry, getDirectory())
	names << dirEntry.name;

	for (int mainIter = 0; mainIter < names.size() - lumpsToCheckFor.size(); ++mainIter)
	{
		bool isMap = true;
		for (int checkIter = 0; checkIter < lumpsToCheckFor.size(); ++checkIter)
		{
			if (names[mainIter + checkIter + 1] != lumpsToCheckFor[checkIter])
			{
				isMap = false;
				break;
			}
		}
		if (isMap)
		{
			maps << names[mainIter];
			mainIter += lumpsToCheckFor.size();
		}
	}
	return maps;
}

DClass<CompressedReader>
{
public:
	QScopedPointer<UnArchive> archive;
	QString filepath;
	QStringList directory;
};

DPointeredNoCopy(CompressedReader)

CompressedReader::CompressedReader(const QString &path)
{
	d->filepath = path;
	d->archive.reset(nullptr);
}

CompressedReader::~CompressedReader()
{
}

bool CompressedReader::load()
{
	d->archive.reset(UnArchive::openArchive(d->filepath));
	if (!d->archive.isNull())
	{
		d->directory = d->archive->files();
	}
	else
		return false;
	return true;
}

QStringList CompressedReader::getAllMaps()
{
	return getAllMapsRootDir();
}

QStringList CompressedReader::getAllMapsRootDir()
{
	QStringList rootPaths;
	QStringList mapList;
	QTemporaryDir tempDir;

	if (tempDir.isValid())
	{
		foreach (QString dirEntry, d->directory)
		{
			if (!dirEntry.contains("/"))
			{
				QString extractedFilePath = tempDir.path() + QDir::separator() + dirEntry;
				QSharedPointer<ModReader> modReader = ModReader::create(extractedFilePath);
				if (!modReader.isNull() && !d->archive.isNull() &&
					d->archive->extract(d->archive->findFileEntry(dirEntry), extractedFilePath))
				{
					modReader->load();
					mapList << modReader->getAllMaps();
				}
			}
		}
	}
	return mapList;
}

PkReader::PkReader(const QString &path) : CompressedReader(path)
{
}

QStringList PkReader::getAllMaps()
{
	QStringList mapList;
	QStringList rootPaths;
	foreach (const QString &dirEntry, d->directory)
	{
		QFileInfo fileInfo(dirEntry);
		if (dirEntry.startsWith("maps/", Qt::CaseInsensitive) &&
			fileInfo.suffix().compare("wad", Qt::CaseInsensitive) == 0)
		{
			mapList << fileInfo.baseName();
		}
	}
	mapList << getAllMapsRootDir();
	return mapList;
}