//------------------------------------------------------------------------------
// odamexserver.h
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

#ifndef __ODAMEXSERVER_H__
#define __ODAMEXSERVER_H__

#include "serverapi/server.h"

#define NUM_ODAMEX_GAME_MODES 5

class OdamexServer : public Server
{
	Q_OBJECT

	public:
		enum OdamexGameModes
		{
			MODE_COOPERATIVE,
			MODE_DEATHMATCH,
			MODE_DEATHMATCH2,
			MODE_TEAM_DEATHMATCH,
			MODE_CAPTURE_THE_FLAG
		};
		static const DMFlagsSection		DM_FLAGS;
		static const GameMode			GAME_MODES[NUM_ODAMEX_GAME_MODES];
		static const QPixmap			*ICON;

		OdamexServer(const QHostAddress &address, unsigned short port);

		QString			clientBinary(QString& error) const;
		void			connectParameters(QStringList &args, PathFinder &pf, bool &iwadFound, const QString &connectPassword) const;

		QString			engineName() const { return tr("Odamex"); }

		const QPixmap	&icon() const;

		QString			serverBinary(QString& error) const;

	protected:
		Response	readRequest(QByteArray &data);
		bool		sendRequest(QByteArray &data);

		short			protocol;
		QStringList		dehPatches;
};

#endif /* __ODAMEXSERVER_H__ */
