//------------------------------------------------------------------------------
// basefileseeker.cpp
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
// Copyright (C) 2013 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#include "basefileseeker.h"

#include "pathfinder/filesearchpath.h"

#include <QDir>
#include <QSet>

DClass<BaseFileSeeker>
{
public:
	QSharedPointer <QList<FileSearchPath> > paths;
};
DPointered(BaseFileSeeker)

BaseFileSeeker::BaseFileSeeker(QSharedPointer <QList<FileSearchPath> > paths)
{
	d->paths = paths;
}

BaseFileSeeker::~BaseFileSeeker()
{
}

QString BaseFileSeeker::findFile(const QString &fileName)
{
	for (int i = 0; i < d->paths->size(); ++i)
	{
		QString result = findFileInPath(fileName, (*d->paths.data())[i]);
		if (!result.isNull())
		{
			return result;
		}
	}
	return QString();
}

QString BaseFileSeeker::findFileInPath(const QString &fileName, FileSearchPath &path)
{
	if (!path.hasCache())
	{
		generatePathCacheAndEditPaths(path);
	}
	if (path.getCache().contains(fileName.toLower()))
		return QDir(path.path()).absoluteFilePath(fileName);
	return QString();
}

void BaseFileSeeker::generatePathCacheAndEditPaths(FileSearchPath &path)
{
	QFileInfoList entriesDirectory = QDir(path.path()).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
	QSet<QString> entriesFileNames, entriesDirAbsolutePath;

	for (const QFileInfo &entry : qAsConst(entriesDirectory))
	{
		if (entry.isFile())
			entriesFileNames << entry.fileName().toLower();
		else if (entry.isDir())
			entriesDirAbsolutePath << entry.absoluteFilePath();
	}

	if (path.isRecursive())
	{
		QList<FileSearchPath> subpaths;
		for (const QString &entry : qAsConst(entriesDirAbsolutePath))
		{
			FileSearchPath subpath = FileSearchPath(entry);
			subpath.setRecursive(true);
			subpaths << subpath;
		}
		insertSubpathsAfterPath(path, subpaths);
		path.setRecursive(false);
	}
	path.setCache(entriesFileNames);
}

void BaseFileSeeker::insertSubpathsAfterPath(const FileSearchPath &path, QList<FileSearchPath> subpaths)
{
	bool startStripping = false;
	QList<FileSearchPath> postPathBaseEntries;
	while (!d->paths->isEmpty() && d->paths->last().path() != path.path())
	{
		postPathBaseEntries.prepend(d->paths->takeLast());
	}
	if (!d->paths->isEmpty())
	{
		postPathBaseEntries.prepend(d->paths->takeFirst());
		*d->paths.data() << subpaths << postPathBaseEntries;
	}
}