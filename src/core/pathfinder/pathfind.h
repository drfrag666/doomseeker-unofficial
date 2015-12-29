//------------------------------------------------------------------------------
// pathfind.h
//------------------------------------------------------------------------------
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//
//------------------------------------------------------------------------------
// Copyright (C) 2015 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef id82392785_6608_4e45_9e65_905e8d266187
#define id82392785_6608_4e45_9e65_905e8d266187

#include <QString>
#include "pathfinder/pathfinder.h"

class GameFile;

namespace PathFind
{
	QString findExe(const PathFinder &pathFinder, const QString &name);
	QString findGameFile(const QStringList &knownPaths, const GameFile &gameFile);
	QString findGameFile(const PathFinder &pathFinder, const GameFile &gameFile);
};

#endif
