//------------------------------------------------------------------------------
// commonGUI.cpp
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
// Copyright (C) 2010 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#include "commonGUI.h"
#include <QInputDialog>
#include <QStandardItemModel>

QString CommonGUI::askString(const QString& title, const QString& label, bool* ok, const QString& defaultString)
{
	return QInputDialog::getText(NULL, title,label, QLineEdit::Normal, defaultString, ok);
}

QStringList CommonGUI::listViewStandardItemsToStringList(QListView* listview)
{
	QStringList list;
	QStandardItemModel* model = static_cast<QStandardItemModel*>(listview->model());
	for (int i = 0; i < model->rowCount(); ++i)
	{
		list << model->item(i)->text();
	}

	return list;
}

void CommonGUI::removeSelectedItemsFromStandardItemView(QAbstractItemView* view)
{
	QItemSelectionModel* selModel = view->selectionModel();
	QModelIndexList indexList = selModel->selectedIndexes();
	selModel->clear();

	QStandardItemModel* model = static_cast<QStandardItemModel*>(view->model());
	QList<QStandardItem*> itemList;
	for (int i = 0; i < indexList.count(); ++i)
	{
		itemList << model->itemFromIndex(indexList[i]);
	}

	for (int i = 0; i < itemList.count(); ++i)
	{
		QModelIndex index = model->indexFromItem(itemList[i]);
		model->removeRow(index.row());
	}
}

void CommonGUI::stringListToStandardItemsListView(QListView* targetListview, const QStringList& stringList)
{
	QStandardItemModel* model = static_cast<QStandardItemModel*>(targetListview->model());
	model->removeRows(0, model->rowCount());

	foreach (const QString& str, stringList)
	{
		QStandardItem* pItem = new QStandardItem();
		pItem->setText(str);
		model->appendRow(pItem);
	}
}
