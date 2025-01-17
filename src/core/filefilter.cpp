//------------------------------------------------------------------------------
// filefilter.cpp
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
// Copyright (C) 2015 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#include "filefilter.h"

QString FileFilter::allFilesFilter()
{
	return tr("All files(*)");
}

QString FileFilter::executableFilesFilter()
{
	#if defined(Q_OS_WIN32)
	return tr("Runnable files (*.exe;*.bat;*.com);;") + allFilesFilter();
	#else
	// Other platforms do not have an extension for their binary files.
	return allFilesFilter();
	#endif
}
