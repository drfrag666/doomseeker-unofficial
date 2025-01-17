//------------------------------------------------------------------------------
// dndfilesystemlistview.cpp
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
#include "dndfilesystemlistview.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QList>
#include <QMimeData>
#include <QUrl>

DndFileSystemListView::DndFileSystemListView(QWidget *pParent)
	: QListView(pParent)
{
}

void DndFileSystemListView::dragEnterEvent(QDragEnterEvent *pEvent)
{
	if (pEvent->mimeData()->hasUrls())
		pEvent->acceptProposedAction();
	else
		QListView::dragEnterEvent(pEvent);
}

void DndFileSystemListView::dropEvent(QDropEvent *pEvent)
{
	if (pEvent->mimeData()->hasUrls())
	{
		QList<QUrl> droppedUrls = pEvent->mimeData()->urls();
		for (const auto &droppedUrl : droppedUrls)
		{
			QString localPath = droppedUrl.toLocalFile();
			QFileInfo fileInfo(localPath);
			if (!localPath.isEmpty() && fileInfo.exists())
				emit fileSystemPathDropped(fileInfo.absoluteFilePath());
		}

		pEvent->acceptProposedAction();
	}
	else
		QListView::dropEvent(pEvent);
}
