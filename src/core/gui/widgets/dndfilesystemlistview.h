//------------------------------------------------------------------------------
// dndfilesystemlistview.h
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
// Copyright (C) 2011 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef __DNDFILESYSTEMLISTVIEW_H__
#define __DNDFILESYSTEMLISTVIEW_H__

#include <QListView>

/**
 * @brief This widget is aware of files and directories drag'n'drop
 *        from external programs.
 *
 * For each valid, existing file system path that is dragged'n'dropped
 * onto this widget, a fileSystemPathDropped() signal will be emitted.
 */
class DndFileSystemListView : public QListView
{
	Q_OBJECT

public:
	DndFileSystemListView(QWidget *pParent = nullptr);

signals:
	void fileSystemPathDropped(const QString &path);

protected:
	void dragEnterEvent(QDragEnterEvent *pEvent) override;
	void dropEvent(QDropEvent *pEvent) override;
};

#endif
