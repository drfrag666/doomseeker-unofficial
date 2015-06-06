//------------------------------------------------------------------------------
// serverlistcounttracker.cpp
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
#include "serverlistcounttracker.h"

#include "serverapi/playerslist.h"
#include "serverapi/server.h"
#include <cassert>

DClass<ServerListCountTracker>
{
public:
	const EnginePlugin *plugin;

	ServerListCount count;
	bool hasRegisterBeenCalled;

	bool isPassingPluginFilter(const ServerPtr &server) const
	{
		return plugin == NULL || server->plugin() == plugin;
	}
};
DPointered(ServerListCountTracker)

ServerListCountTracker::ServerListCountTracker(QObject *parent)
: QObject(parent)
{
	// {} intializes the struct with zeroes
	// http://stackoverflow.com/questions/17080975/c-correct-way-of-initializing-a-struct-in-a-class-constructor
	d->count = {};
	d->plugin = NULL;
	d->hasRegisterBeenCalled = false;
}

const ServerListCount &ServerListCountTracker::count() const
{
	return d->count;
}

void ServerListCountTracker::setPluginFilter(const EnginePlugin *plugin)
{
	assert(!d->hasRegisterBeenCalled && "don't change filter after server has already been registered");
	d->plugin = plugin;
}

void ServerListCountTracker::deregisterServer(ServerPtr server)
{
	if (d->isPassingPluginFilter(server))
	{
		server->disconnect(this);
		d->count.discountServer(server);
		if (server->isRefreshing())
		{
			--d->count.numRefreshing;
		}
		else
		{
			d->count.discountPlayers(server);
		}
		emit updated();
	}
}

void ServerListCountTracker::registerServer(ServerPtr server)
{
	d->hasRegisterBeenCalled = true;
	if (d->isPassingPluginFilter(server))
	{
		this->connect(server.data(), SIGNAL(begunRefreshing(ServerPtr)),
			SLOT(onServerBegunRefreshing(ServerPtr)));
		this->connect(server.data(), SIGNAL(updated(ServerPtr, int)),
			SLOT(onServerUpdated(ServerPtr)));

		d->count.countServer(server);
		if (server->isRefreshing())
		{
			++d->count.numRefreshing;
		}
		else
		{
			d->count.countPlayers(server);
		}
		emit updated();
	}
}

void ServerListCountTracker::onServerBegunRefreshing(ServerPtr server)
{
	d->count.discountPlayers(server);
	++d->count.numRefreshing;
	emit updated();
}

void ServerListCountTracker::onServerUpdated(ServerPtr server)
{
	d->count.countPlayers(server);
	--d->count.numRefreshing;
	emit updated();
}

///////////////////////////////////////////////////////////////////////////

void ServerListCount::countPlayers(const ServerPtr &server)
{
	const PlayersList &players = server->players();

	numBots += players.numBots();
	numHumanPlayers += players.numClientsWithoutBots();
	numPlayers += players.numClients();
}

void ServerListCount::discountPlayers(const ServerPtr &server)
{
	const PlayersList &players = server->players();

	numBots -= players.numBots();
	numHumanPlayers -= players.numClientsWithoutBots();
	numPlayers -= players.numClients();
}

void ServerListCount::countServer(const ServerPtr &server)
{
	++numServers;
	numGenericServers += !server->isSpecial() ? 1 : 0;
	numLanServers += server->isLan() ? 1 : 0;
	numCustomServers += server->isCustom() ? 1 : 0;
}

void ServerListCount::discountServer(const ServerPtr &server)
{
	--numServers;
	numGenericServers -= !server->isSpecial() ? 1 : 0;
	numLanServers -= server->isLan() ? 1 : 0;
	numCustomServers -= server->isCustom() ? 1 : 0;
}

int ServerListCount::refreshedPercent() const
{
	if (numRefreshing == 0 || numServers == 0)
	{
		return 100;
	}
	else
	{
		float refreshingFactor = static_cast<float>(numRefreshing) /
			static_cast<float>(numServers);
		return static_cast<int>(floor(100.0 - 100.0 * refreshingFactor));
	}
}
