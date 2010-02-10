//------------------------------------------------------------------------------
// serverlist.cpp
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
// Copyright (C) 2009 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------

#include "gui/remoteconsole.h"
#include "gui/serverlist.h"
#include "gui/models/serverlistcolumn.h"
#include "gui/models/serverlistrowhandler.h"
#include "gui/widgets/serverlistcontextmenu.h"
#include "serverapi/tooltipgenerator.h"
#include "main.h"
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QStandardItem>
#include <QToolTip>

using namespace ServerListColumnId;

ServerListHandler::ServerListHandler(ServerListView* serverTable, Config* config, QWidget* pMainWindow)
: configuration(config), mainWindow(pMainWindow), model(NULL),
  needsCleaning(false), sortingProxy(NULL), sortOrder(Qt::AscendingOrder),
  sortIndex(-1), table(serverTable)
{
	// We will be saving the column widths to the config, but lets get the defaults
	initDefaultColumnsWidthsSettings();

	// Now we see if we have a different configuration.
	loadColumnsWidthsSettings();

	prepareServerTable();

	initCleanerTimer();
}

ServerListHandler::~ServerListHandler()
{
	saveColumnsWidthsSettings();
}

bool ServerListHandler::areColumnsWidthsSettingsChanged()
{
	for(int i = 0; i < NUM_SERVERLIST_COLUMNS; ++i)
	{
		if(ServerListColumns::columns[i].width != table->columnWidth(i))
		{
			return true;
		}
	}

	return false;
}

void ServerListHandler::clearTable()
{
	model->destroyRows();
}

void ServerListHandler::cleanUp()
{
	if (needsCleaning && mainWindow->isActiveWindow())
	{
		if (sortIndex >= 0)
		{
			ServerListSortFilterProxyModel* pModel = static_cast<ServerListSortFilterProxyModel*>(table->model());
			pModel->invalidate();
			pModel->sortServers(sortIndex, sortOrder);
		}

		setCountryFlagsIfNotPresent();
		table->updateAllRows();
		needsCleaning = false;
	}
}

void ServerListHandler::columnHeaderClicked(int index)
{
	if (isSortingByColumn(index))
	{
		sortOrder = swapCurrentSortOrder();
	}
	else
	{
		sortOrder = getColumnDefaultSortOrder(index);
	}
	sortIndex = index;

	needsCleaning = true;
	cleanUp();

	QHeaderView* header = table->horizontalHeader();
	header->setSortIndicator(sortIndex, sortOrder);

	//table->fixRowSize();
}

void ServerListHandler::connectTableModelProxySlots()
{
	QHeaderView* header = table->horizontalHeader();
	connect(header, SIGNAL( sectionClicked(int) ), this, SLOT ( columnHeaderClicked(int) ) );
	connect(sortingProxy, SIGNAL( rowsInserted(const QModelIndex&, int, int) ), this, SLOT( resizeChangedRows(const QModelIndex&, int, int) ));
	connect(model, SIGNAL( modelCleared() ), this, SLOT( modelCleared() ) );
	connect(table, SIGNAL( clicked(const QModelIndex&) ), this, SLOT( itemSelected(const QModelIndex&) ));
	connect(table, SIGNAL( rightMouseClick(const QModelIndex&, const QPoint&) ), this, SLOT ( itemSelected(const QModelIndex&)) );
	connect(table, SIGNAL( rightMouseClick(const QModelIndex&, const QPoint&) ), this, SLOT ( tableRightClicked(const QModelIndex&, const QPoint&)) );
	connect(table, SIGNAL( entered(const QModelIndex&) ), this, SLOT ( mouseEntered(const QModelIndex&)) );
	connect(table, SIGNAL( leftMouseDoubleClicked(const QModelIndex&, const QPoint&)), this, SLOT( doubleClicked(const QModelIndex&)) );
}

QString ServerListHandler::createPlayersToolTip(const Server* server)
{
	if (server == NULL || !server->isKnown())
	{
		return QString();
	}

	TooltipGenerator* tooltipGenerator = server->tooltipGenerator();

    QString ret;
	ret = "<div style='white-space: pre'>";
	ret += tooltipGenerator->gameInfoTableHTML();
	if(server->numPlayers() != 0)
	{
		ret += tooltipGenerator->playerTableHTML();
	}
	ret += "</div>";

	delete tooltipGenerator;
	return ret;
}

QString ServerListHandler::createServerNameToolTip(const Server* server)
{
	if (server == NULL)
	{
		return QString();
	}

	TooltipGenerator* tooltipGenerator = server->tooltipGenerator();

	QString ret;
	QString generalInfo = tooltipGenerator->generalInfoHTML();

	if (!generalInfo.isEmpty())
	{
		ret = "<div style='white-space: pre'>";
		ret += generalInfo;
		ret += "</div>";
	}

	delete tooltipGenerator;
	return ret;
}

ServerListModel* ServerListHandler::createModel()
{
	ServerListModel* serverListModel = new ServerListModel(this);
	serverListModel->prepareHeaders();

	return serverListModel;
}

QSortFilterProxyModel* ServerListHandler::createSortingProxy(ServerListModel* serverListModel)
{
	QSortFilterProxyModel* proxy = new ServerListSortFilterProxyModel(this);
	proxy->setSourceModel(serverListModel);
	proxy->setSortRole(ServerListModel::SLDT_SORT);
	proxy->setSortCaseSensitivity( Qt::CaseInsensitive );
	proxy->setFilterKeyColumn(IDServerName);

	return proxy;
}

QString ServerListHandler::createPwadsToolTip(const Server* server)
{
	if (server == NULL || !server->isKnown() || server->numWads() == 0)
	{
		return QString();
	}

	QString ret;
	ret = "<div style='white-space: pre'>";
	ret += server->pwads().join("\n");
	ret += "</div>";
	return ret;
}

void ServerListHandler::doubleClicked(const QModelIndex& index)
{
	emit serverDoubleClicked(serverFromIndex(index));
}

Qt::SortOrder ServerListHandler::getColumnDefaultSortOrder(int columnId)
{
	// Right now we can assume that columnIndex == columnId.
	return ServerListColumns::columns[columnId].defaultSortOrder;
}

void ServerListHandler::initCleanerTimer()
{
	cleanerTimer.setInterval(200);
	cleanerTimer.start();
	connect(&cleanerTimer, SIGNAL( timeout() ), this, SLOT ( cleanUp() ) );
}

void ServerListHandler::initDefaultColumnsWidthsSettings()
{
	ServerListColumn* columns = ServerListColumns::columns;

	QString widths;
	for(int i = 0; i < NUM_SERVERLIST_COLUMNS; ++i)
	{
		if(i != 0)
		{
			widths += ",";
		}
		widths += QString("%1").arg(columns[i].width);
	}
	configuration->createSetting("ServerListColumnWidths", widths.toAscii().data());
}

bool ServerListHandler::isSortingByColumn(int columnIndex)
{
	return sortIndex == columnIndex;
}

void ServerListHandler::itemSelected(const QModelIndex& index)
{
	QList<Server*> servers = selectedServers();
	emit serversSelected(servers);
}

void ServerListHandler::loadColumnsWidthsSettings()
{
	ServerListColumn* columns = ServerListColumns::columns;

	QStringList colWidths = configuration->setting("ServerListColumnWidths")->string().split(',', QString::SkipEmptyParts);
	if(colWidths.size() == NUM_SERVERLIST_COLUMNS) // If the number of columns do not match than reset this setting
	{
		for(int i = 0;i < NUM_SERVERLIST_COLUMNS;i++)
		{
			bool ok = false;
			int width = colWidths[i].toInt(&ok);
			if(ok)
			{
				columns[i].width = width;
			}
		}
	}
}

void ServerListHandler::modelCleared()
{
	QList<Server*> servers;
	emit serversSelected(servers);
}

void ServerListHandler::mouseEntered(const QModelIndex& index)
{
	QSortFilterProxyModel* pModel = static_cast<QSortFilterProxyModel*>(table->model());
	QModelIndex realIndex = pModel->mapToSource(index);
	Server* server = model->serverFromList(realIndex);
	QString tooltip;

	// Functions inside cases perform checks on the server structure
	// to see if any tooltip should be generated. Empty string is returned
	// in case if it should be not.
	switch(index.column())
	{
		case IDPlayers:
			tooltip = createPlayersToolTip(server);
			break;

		case IDServerName:
			tooltip = createServerNameToolTip(server);
			break;

		case IDWads:
			tooltip = createPwadsToolTip(server);
			break;

		default:
			tooltip = "";
			break;
	}

	QToolTip::showText(QCursor::pos(), tooltip, table);
}

void ServerListHandler::prepareServerTable()
{
	model = createModel();
	sortingProxy = createSortingProxy(model);
	setupTableProperties(sortingProxy);

	connectTableModelProxySlots();

	columnHeaderClicked(IDPlayers);
}

void ServerListHandler::redraw()
{
	model->redrawAll();
}

void ServerListHandler::refreshAll()
{
	for (int i = 0; i < model->rowCount(); ++i)
	{
		Server* serv = model->serverFromList(i);
		serv->refresh();
	}
}

void ServerListHandler::refreshSelected()
{
	QItemSelectionModel* selectionModel = table->selectionModel();
	QModelIndexList indexList = selectionModel->selectedRows();

	for(int i = 0; i < indexList.count(); ++i)
	{
		QModelIndex realIndex = sortingProxy->mapToSource(indexList[i]);
		Server* server = model->serverFromList(realIndex);
		server->refresh();
	}
}

void ServerListHandler::resizeChangedRows(const QModelIndex &parent, int start, int end)
{
	// This is so when the search is undone the rows don't become fat again.
	for (int i = start; i < end; ++i)
	{
		table->resizeRowToContents(i);
	}
}

void ServerListHandler::saveColumnsWidthsSettings()
{
	if(areColumnsWidthsSettingsChanged())
	{
		QString widths;
		for(int j = 0; j < NUM_SERVERLIST_COLUMNS; ++j)
		{
			if(j != 0)
			{
				widths += ",";
			}

			widths += QString("%1").arg(table->columnWidth(j));
		}
		configuration->setting("ServerListColumnWidths")->setValue(widths);
	}
}

QList<Server*> ServerListHandler::selectedServers()
{
	QSortFilterProxyModel* pModel = static_cast<QSortFilterProxyModel*>(table->model());
	QItemSelectionModel* selModel = table->selectionModel();
	QModelIndexList indexList = selModel->selectedRows();

	QList<Server*> servers;
	for(int i = 0; i < indexList.count(); ++i)
	{
		QModelIndex realIndex = pModel->mapToSource(indexList[i]);
		Server* server = model->serverFromList(realIndex);
		servers.append(server);
	}
	return servers;
}

void ServerListHandler::serverBegunRefreshing(Server* server)
{
	model->setRefreshing(server);
}

Server *ServerListHandler::serverFromIndex(const QModelIndex &index)
{
	QSortFilterProxyModel* pModel = static_cast<QSortFilterProxyModel*>(table->model());
	QModelIndex indexReal = pModel->mapToSource(index);
	return model->serverFromList(indexReal);
}

void ServerListHandler::serverUpdated(Server *server, int response)
{
	int rowIndex = model->findServerOnTheList(server);
	if (rowIndex >= 0)
	{
		rowIndex = model->updateServer(rowIndex, server, response);
	}
	else
	{
		rowIndex = model->addServer(server, response);
	}

	table->resizeRowToContents(rowIndex);

	needsCleaning = true;
}

void ServerListHandler::setCountryFlagsIfNotPresent()
{
	const bool FORCE = true;
	updateCountryFlags(!FORCE);
}

void ServerListHandler::setupTableColumnWidths()
{
	for (int i = 0; i < NUM_SERVERLIST_COLUMNS; ++i)
	{
		ServerListColumn* columns = ServerListColumns::columns;
		table->setColumnWidth(i, columns[i].width);
		table->setColumnHidden(i, columns[i].bHidden);
		if(!columns[i].bResizable)
		{
			table->horizontalHeader()->setResizeMode(i, QHeaderView::Fixed);
		}
	}
}

void ServerListHandler::setupTableProperties(QSortFilterProxyModel* tableModel)
{
	table->setModel(tableModel);
	table->setIconSize(QSize(26, 15));
	// We don't really need a vertical header so lets remove it.
	table->verticalHeader()->hide();
	// Some flags that can't be set from the Designer.
	table->horizontalHeader()->setSortIndicatorShown(true);
	table->horizontalHeader()->setHighlightSections(false);

	table->setMouseTracking(true);

	setupTableColumnWidths();
}

Qt::SortOrder ServerListHandler::swapCurrentSortOrder()
{
	if (sortOrder == Qt::AscendingOrder)
	{
		return Qt::DescendingOrder;
	}
	else
	{
		return Qt::AscendingOrder;
	}
}


void ServerListHandler::tableRightClicked(const QModelIndex& index, const QPoint& point)
{
	Server* server = serverFromIndex(index);

	ServerListContextMenu contextMenu(server);

	QPoint displayPoint = table->viewport()->mapToGlobal(point);
	ServerListContextMenu::Result contextMenuResult = contextMenu.exec(displayPoint);

	switch (contextMenuResult)
	{
		case ServerListContextMenu::DataCopied:
			// Do nothing.
			break;

		case ServerListContextMenu::Join:
			emit serverDoubleClicked(server);
			break;

		case ServerListContextMenu::OpenRemoteConsole:
			new RemoteConsole(server);
			break;

		case ServerListContextMenu::OpenURL:
			QDesktopServices::openUrl(server->website());
			break;

		case ServerListContextMenu::NothingHappened:
			// Do nothing; ignore.
			break;

		case ServerListContextMenu::Refresh:
			refreshSelected();
			break;

		case ServerListContextMenu::ShowJoinCommandLine:
			emit displayServerJoinCommandLine(server);
			break;

		default:
			QMessageBox::warning(mainWindow, tr("Doomseeker - context menu warning"), tr("Unhandled behavior int ServerListHandler::tableRightClicked()"));
			break;
	}
}

void ServerListHandler::updateCountryFlags()
{
	const bool FORCE = true;
	updateCountryFlags(FORCE);
}

void ServerListHandler::updateCountryFlags(bool force)
{
    for (int i = 0; i < model->rowCount(); ++i)
    {
        model->updateFlag(i, force);
    }
}

void ServerListHandler::updateSearch(const QString& search)
{
	QRegExp pattern(QString("*") + search + "*", Qt::CaseInsensitive, QRegExp::Wildcard);
	sortingProxy->setFilterRegExp(pattern);
}
