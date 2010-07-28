//------------------------------------------------------------------------------
// ircdock.cpp
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
#include "ircdock.h"
#include "gui/irc/ircdocktabcontents.h"
#include "gui/irc/ircnetworkselectionbox.h"
#include "irc/ircnetworkadapter.h"

#include <QToolBar>

IRCDock::IRCDock(QWidget* parent)
: QDockWidget(parent)
{
	setupUi(this);
	setupToolbar();
}

void IRCDock::setupToolbar()
{
	QToolBar* pToolBar = new QToolBar(this);
	pToolBar->setMovable(false);

	toolBarConnect = new QAction(QIcon(":/icons/network-connect-3.png"), tr("Connect"), pToolBar);
	toolBarDisconnect = new QAction(QIcon(":/icons/network-disconnect-3.png"), tr("Disconnect"), pToolBar);

	pToolBar->addAction(toolBarConnect);
	pToolBar->addAction(toolBarDisconnect);

	verticalLayout->insertWidget(0, pToolBar);
	connect(pToolBar, SIGNAL( actionTriggered(QAction*) ), this, SLOT( toolBarAction(QAction*) ) );
}

void IRCDock::toolBarAction(QAction* pAction)
{
	if (pAction == toolBarConnect)
	{
		IRCNetworkSelectionBox networkSelection(this);
		if (networkSelection.exec() == QDialog::Accepted)
		{
			IRCNetworkConnectionInfo connectionInfo;
			networkSelection.networkConnectionInfo(connectionInfo);
			
			IRCNetworkAdapter* pIRCNetworkAdapter = new IRCNetworkAdapter();

			// Setup the UI tab for the new network.
			IRCDockTabContents* pNewConnectionWidget = new IRCDockTabContents(this);
			pNewConnectionWidget->setIRCAdapter(pIRCNetworkAdapter);
			
			// Connect to the network.
			pIRCNetworkAdapter->connect(connectionInfo);
			
			tabWidget->addTab(pNewConnectionWidget, connectionInfo.serverAddress);
		}
	}
}
