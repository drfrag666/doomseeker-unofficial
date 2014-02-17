//------------------------------------------------------------------------------
// serverlist.h
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

#ifndef __SERVERLIST_H_
#define __SERVERLIST_H_

#include <QObject>
#include <QStandardItem>
#include <QString>
#include <QTableView>
#include <QTimer>

#include "gui/widgets/serverlistview.h"
#include "gui/models/serverlistmodel.h"
#include "serverapi/serverptr.h"

class PWad;
class IniSection;
class Server;
class ServerListFilterInfo;

class ServerListHandler : public QObject
{
	Q_OBJECT

	public:
		ServerListHandler(ServerListView* serverTable, QWidget* pMainWindow);
		~ServerListHandler();

		void 				clearTable();
		void				cleanUpForce();

		QWidget*			getMainWindow() { return mainWindow; }
		bool				hasAtLeastOneServer() const;

		bool				isSortingByColumn(int columnIndex);

		QList<ServerPtr> selectedServers();

		ServerPtr serverFromIndex(const QModelIndex&);

		ServerListModel*	serverModel() { return model; }
		ServerListView*		serverTable() { return table; }

	public slots:
		void				applyFilter(const ServerListFilterInfo& filterInfo);
		void 				cleanUp();
		/**
		 * @brief Looks up hosts for all available servers.
		 */
		void				lookupHosts();
		void 				redraw();
		void 				refreshAll();
		void				refreshSelected();
		void 				serverBegunRefreshing(ServerPtr server);
		void 				serverUpdated(ServerPtr server, int response);

		/**
		 *	@brief Sets country flags for servers that don't have flags
		 *		present yet.
		 */
		void				setCountryFlagsIfNotPresent();

		void				tableMiddleClicked(const QModelIndex& index, const QPoint& cursorPosition);
		void 				tableRightClicked(const QModelIndex& index, const QPoint& cursorPosition);
		void 				updateCountryFlags();
		void 				updateSearch(const QString& search);

	protected slots:
		/// Handles column sorting.
		void 				columnHeaderClicked(int);
		void 				doubleClicked(const QModelIndex&);
		void 				itemSelected(const QModelIndex&);
		void 				modelCleared();
		void 				mouseEntered(const QModelIndex&);

	signals:
		/**
		 * Emitted when a request for join command line show is called.
		 */
		void displayServerJoinCommandLine(ServerPtr);

		void serverFilterModified(const ServerListFilterInfo& filter);
		/**
		 *	@brief Emitted every time when a server info is updated through
		 *	serverUpdated()
		 */
		void serverInfoUpdated(ServerPtr);
		void serverDoubleClicked(ServerPtr);
		void serversSelected(QList<ServerPtr>&);

	protected:
		// TODO: These need to be set by appearance configuration.
		static const QString	FONT_COLOR_MISSING;
		static const QString	FONT_COLOR_OPTIONAL;
		static const QString	FONT_COLOR_FOUND;

		QTimer					cleanerTimer;

		QWidget*				mainWindow;
		ServerListModel* 		model;
		bool					needsCleaning;
		QSortFilterProxyModel*	sortingProxy;

		Qt::SortOrder 			sortOrder;
		int						sortIndex;
		ServerListView*			table;

		QString					createIwadToolTip(ServerCPtr server);
		QString 				createPlayersToolTip(ServerCPtr server);
		QString					createPortToolTip(ServerCPtr server);
		QString 				createPwadsToolTip(ServerCPtr server);
		QString					createPwadToolTipInfo(const PWad& pwad, const QString& binPath);
		QString 				createServerNameToolTip(ServerCPtr server);

		bool					areColumnsWidthsSettingsChanged();

		void					connectTableModelProxySlots();

		ServerListModel*		createModel();
		QSortFilterProxyModel*	createSortingProxy(ServerListModel* serverListModel);

		Qt::SortOrder			getColumnDefaultSortOrder(int columnId);

		void					initCleanerTimer();

		void 					prepareServerTable();

		void					saveColumnsWidthsSettings();

		void					setupTableColumnWidths();
		void					setupTableProperties(QSortFilterProxyModel* tableModel);

		Qt::SortOrder			swapCurrentSortOrder();

		void 					updateCountryFlags(bool force);
};

#endif
