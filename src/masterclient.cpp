//------------------------------------------------------------------------------
// masterclient.cpp
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
// Copyright (C) 2009 "Blzut3" <admin@maniacsvault.net>
//------------------------------------------------------------------------------

#include "log.h"
#include "masterclient.h"
#include "main.h"
#include "serverapi/playerslist.h"

#include <QDataStream>
#include <QErrorMessage>
#include <QHostInfo>
#include <QMessageBox>
#include <QUdpSocket>

QUdpSocket* MasterClient::pGlobalUdpSocket = NULL;

MasterClient::MasterClient() : QObject(), cache(NULL)
{
}

MasterClient::~MasterClient()
{
	emptyServerList();
	resetPacketCaching();
}

void MasterClient::emptyServerList()
{
	for(int i = 0;i < servers.size();i++)
	{
		servers[i]->disconnect();
		servers[i]->setToDelete(true);
	}
	servers.clear();
}

bool MasterClient::hasServer(const Server* server)
{
	for (int i = 0; i < servers.count(); ++i)
	{
		if (server == servers[i])
			return true;
	}

	return false;
}

void MasterClient::notifyBanned(const QString& engineName)
{
	emit message(engineName, tr("You have been banned from the master server."), true);
}

void MasterClient::notifyDelay(const QString& engineName)
{
	emit message(engineName, tr("Could not fetch a new server list from the master because not enough time has past."), true);
}

void MasterClient::notifyUpdate(const QString& engineName)
{
	emit message(engineName, tr("Could not fetch a new server list.  The protocol you are using is too old.  An update may be available."), true);
}

int MasterClient::numPlayers() const
{
	int players = 0;
	foreach(Server* server, servers)
	{
		if (server != NULL)
		{
			players += server->playersList()->numClients();
		}
	}

	return players;
}

bool MasterClient::preparePacketCache(bool write)
{
	if(cache == NULL)
	{
		if(plugin() == NULL)
			return false;

		QString cacheFile(Main::dataPaths->programsDataDirectoryPath() + "/" + QString(plugin()->name).replace(' ', ""));
		cache = new QFile(cacheFile);
		if(!cache->open(write ? QIODevice::WriteOnly|QIODevice::Truncate : QIODevice::ReadOnly))
		{
			resetPacketCaching();
			return false;
		}
	}
	return cache != NULL;
}

void MasterClient::pushPacketToCache(QByteArray &data)
{
	if(!preparePacketCache(true))
		return;

	QDataStream strm(cache);
	strm << static_cast<quint16>(data.size());
	strm << data;
}

void MasterClient::readPacketCache()
{
	if(!preparePacketCache(false))
		return;

	gLog << tr("Reloading master server results from cache for %1!").arg(plugin()->name);
	QDataStream strm(cache);
	while(!strm.atEnd())
	{
		quint16 size;
		strm >> size;

		QByteArray data(size, '\0');
		strm >> data;

		if(!readMasterResponse(data))
			break;
	}
}

void MasterClient::resetPacketCaching()
{
	if(cache != NULL)
	{
		delete cache;
		cache = NULL;
	}
}

void MasterClient::refresh()
{
	bTimeouted = false;
	emptyServerList();

	if(address.isNull())
		return;

	// Make request
	QByteArray request;
	if(!getServerListRequest(request))
	{
		return;
	}
	pGlobalUdpSocket->writeDatagram(request, address, port);
}

void MasterClient::timeoutRefresh()
{
	// Avoid timeouting more than once. This would cause errors.
	if (!bTimeouted)
	{
		bTimeouted = true;

		emit message(tr("Master server timeout"), tr("Connection timeout (%1:%2).").arg(address.toString()).arg(port), true);
		readPacketCache();

		timeoutRefreshEx();
		listUpdated();
	}
}

void MasterClient::updateAddress()
{
	QString host;
	unsigned short port;
	plugin()->pInterface->masterHost(host, port);

	QHostInfo info = QHostInfo::fromName(host);
	if(info.addresses().size() == 0)
		return;

	this->address = info.addresses().first();
	if(this->address.protocol() != QAbstractSocket::IPv4Protocol)
	{
		foreach(const QHostAddress &addr, info.addresses())
		{
			if(addr.protocol() == QAbstractSocket::IPv4Protocol)
				this->address = addr;
		}
	}
	this->port = port;
}
