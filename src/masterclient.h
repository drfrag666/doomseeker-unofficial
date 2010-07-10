//------------------------------------------------------------------------------
// masterclient.h
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

#ifndef __MASTERSERVER_H__
#define __MASTERSERVER_H__

#include <QObject>
#include <QHostAddress>
#include <QList>

#include "serverapi/server.h"

struct PluginInfo;

/**
 * Abstract class base for all MasterClients.  This is expected to fetch a list
 * of IP addresses which will be turned into Servers.
 */
class MAIN_EXPORT MasterClient : public QObject
{
	Q_OBJECT

	public:
		/**
		 *	@brief Generic Doomseeker's socket used for network communication.
		 *
		 *	If this is not NULL plugins may use this socket to send UDP packets.
		 *	In fact the default implementation of MasterClient::refresh() method
		 *	will use this socket in order to send the challenge data.
		 *	In this case any responses that arrive on this socket will 
		 *	automatically be redirected to appropriate master client instance
		 *	by the Doomseeker's Refreshing thread.
		 *
		 *	If engine requires a customized way to obtain the server list it 
		 *	must be implemented within the plugin itself.
		 */
		static QUdpSocket*		pGlobalUdpSocket;
	
		MasterClient(QHostAddress address, unsigned short port);
		virtual ~MasterClient();
		
		bool					hasServer(const Server*);		
		
		/**
		 *	@brief Returns true if the passed address:port is the same as this
		 *	master server's.
		 */
		bool					isAddressDataCorrect(const QHostAddress& address, unsigned short port)
		{
			return (this->address == address && this->port == port);
		}
		
		/**
		 *	Serves as an informative role for MasterManager.
		 *	If the master client is disabled, master manager will omit
		 *	it during the refresh.
		 */
		bool					isEnabled() const { return enabled; }
		bool					isTimeouted() const { return bTimeouted; }
		int						numPlayers() const;
		int						numServers() const { return servers.size(); }
		Server					*operator[] (int index) const { return servers[index]; }

		/**
		 *	This is supposed to return the plugin this MasterClient belongs to.
		 *	If it doesn't belong to any plugin then return NULL.
		 *	New instances of PluginInfo shouldn't be created here. Instead
		 *	each plugin should keep a global instance of PluginInfo (singleton?)
		 *	and a pointer to this instance should be returned.
		 */
		virtual const PluginInfo*		plugin() const = 0;
		
		/**
		 *	@brief Called to read and analyze the response from the 
		 *	MasterServer.
		 *
		 *	@return If false is returned refreshing of this master server is
		 *	immediatelly aborted.
		 */
		virtual bool			readMasterResponse(QByteArray &data)=0;	
		
		/**
		 *	@brief Reads master response only if address and port are of this 
		 *	server.
		 *
		 *	Reimplemented by MasterManager.
		 */	
		virtual bool			readMasterResponse(QHostAddress& address, unsigned short port, QByteArray &data)
		{
			if (isAddressDataCorrect(address, port))
			{
				return readMasterResponse(data);
			}
			
			return false;
		}

		QList<Server*>			&serverList() { return servers; }
		const QList<Server*>	&serverList() const { return servers; }

	public slots:
		/**
		 *	Requests an updated server list from the master.
		 *
		 *	This function is virtual since MasterManager overrides it.
		 */
		virtual void			refresh();

		/**
		 *	@see isEnabled()
		 */
		void					setEnabled(bool b) { enabled = b; }
		
		/**
		 *	@brief Times the refreshing process out.
		 *
		 *	This calls timeoutRefreshEx() and then emits listUpdated() signal.
		 */
		void					timeoutRefresh();

	signals:
		void					listUpdated();
	
		/**
		 *	Plugins may use this to make Doomseeker display custom messages.
		 *	Messages are dumped into the log.
		 */
		void					message(const QString& title, const QString& content, bool isError);
		
		/**
		 *	@brief Emit this signal each time a new batch of servers is 
		 *	received.
		 *
		 *	This signal should be called by the plugin after the response packet
		 *	delivered to readMasterResponse() is processed. Master servers
		 *	that send their response in multiple packets should be handled
		 *	nicely by this.
		 */
		void					newServerBatchReceived(const QList<Server* >& servers);
		
	protected:
		QHostAddress			address;
		
		/**
		 *	@brief Indicates that the server has timeouted recently.
		 *
		 *	This is reset to false by refresh() and set to true by 
		 *	timeoutRefresh(). If you reimplement refresh() please remember
		 *	to set this to false.
		 */
		bool					bTimeouted;
		bool					enabled;
		unsigned short			port;
		QList<Server *>			servers;

		/**
		 * Clears the server list.
		 */
		void					emptyServerList();
		
		/**
		 *	@brief Method that is supposed to produce the contents of server 
		 *	list request packet that is sent to the master server.
		 *
		 *	@return If false is returned refreshing of this master server is
		 *	immediatelly aborted.
		 */
		virtual bool			getServerListRequest(QByteArray &data)=0;		
		
		/**
		 * Informs the user that they have been banned from the master server.
		 */
		void					notifyBanned(const QString& engineName);
		/**
		 * Tells the user that the master server will not respond to their
		 * query becuase they tried to refresh too quickly.
		 */
		void					notifyDelay(const QString& engineName);
		/**
		 * Tells the user they need to update since the protocol is too old.
		 */
		void					notifyUpdate(const QString& engineName);
		
		/**
		 *	@brief Reimplement this for clean up purposes.
		 */
		virtual void			timeoutRefreshEx() {}
};

#endif /* __MASTERSERVER_H__ */
