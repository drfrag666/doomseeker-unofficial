//------------------------------------------------------------------------------
// casesensitivefsfileseeker.cpp
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
#include "casesensitivefsfileseeker.h"

#include "pathfinder/filesearchpath.h"

#include <QDir>
#include <QFileInfo>
#include <QSet>
#include <QStringList>

CaseSensitiveFSFileSeeker::CaseSensitiveFSFileSeeker(QSharedPointer <QList<FileSearchPath> > paths) :
	FileSeeker(paths)
{
}

QString CaseSensitiveFSFileSeeker::findFileInPath(const QString &fileName, FileSearchPath &path)
{
	if (!path.hasCache())
		generatePathCacheAndEditPaths(path);
	if (path.getCache().contains(fileName.toLower()))
		return QDir(path.path()).absoluteFilePath(fileName);
	return QString();
}

void CaseSensitiveFSFileSeeker::generatePathCacheAndEditPaths(FileSearchPath &path)
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
		for (const QString &entry : entriesDirAbsolutePath)
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

void CaseSensitiveFSFileSeeker::insertSubpathsAfterPath(const FileSearchPath &path, QList<FileSearchPath> subpaths)
{
	QList<FileSearchPath> postPathBaseEntries;
	while (!paths->isEmpty() && paths->last().path() != path.path())
		postPathBaseEntries.prepend(paths->takeLast());
	if (!paths->isEmpty())
	{
		postPathBaseEntries.prepend(paths->takeFirst());
		*paths.data() << subpaths << postPathBaseEntries;
	}
}