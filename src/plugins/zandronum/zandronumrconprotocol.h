//------------------------------------------------------------------------------
// zandronumrconprotocol.h
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
// Copyright (C) 2014 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef idd10651d2_c61d_498e_8ff6_88a782a8fa1c
#define idd10651d2_c61d_498e_8ff6_88a782a8fa1c

#include "serverapi/rconprotocol.h"
#include <QTimer>

class ZandronumRConProtocol : public RConProtocol
{
	Q_OBJECT

	private:
		enum
		{
			SVRCU_PLAYERDATA = 0,
			SVRCU_ADMINCOUNT,
			SVRCU_MAP,

			SVRC_OLDPROTOCOL = 32,
			SVRC_BANNED,
			SVRC_SALT,
			SVRC_LOGGEDIN,
			SVRC_INVALIDPASSWORD,
			SVRC_MESSAGE,
			SVRC_UPDATE,

			CLRC_BEGINCONNECTION = 52,
			CLRC_PASSWORD,
			CLRC_COMMAND,
			CLRC_PONG,
			CLRC_DISCONNECT
		};

	public:
		static RConProtocol *connectToServer(ServerPtr server);

	public slots:
		void disconnectFromServer();
		void sendCommand(const QString &cmd);
		void sendPassword(const QString &password);
		void sendPong();

	protected:
		ZandronumRConProtocol(ServerPtr server);

		void processPacket(QIODevice* ioDevice, bool initial=false, int maxUpdates=1);

		QTimer pingTimer;
		QString hostName;
		QString salt;
		int serverProtocolVersion;

	protected slots:
		void packetReady();
};

#endif