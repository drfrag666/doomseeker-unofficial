//------------------------------------------------------------------------------
// serverpassword.h
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
// Copyright (C) 2013 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef id279D1E56_A461_424E_93E3EF155BEFA077
#define id279D1E56_A461_424E_93E3EF155BEFA077

#include <QDateTime>
#include <QString>
#include <QVariant>

#include "serverapi/serversummary.h"

class ServerPassword
{
	public:
		static ServerPassword deserializeQVariant(const QVariant& map);

		/**
		 * @brief Password is valid if its phrase is not an empty string.
		 *
		 * Technically passwords full of whitespace are also valid.
		 */
		bool isValid() const
		{
			return !d.phrase.isEmpty();
		}

		QString lastGame() const;
		ServerSummary lastServer() const;
		QString lastServerName() const;
		QDateTime lastTime() const;

		ServerSummary mostSimilarServer(const ServerSummary& other, float* outSimilarity = NULL) const;

		const QList<ServerSummary>& servers() const
		{
			return d.servers;
		}

		const QString& phrase() const
		{
			return d.phrase;
		}

		QVariant serializeQVariant() const;

		void addServer(const ServerSummary& v);
		void removeServer(const QString& game, const QString& address, unsigned short port);

		void setPhrase(const QString& v)
		{
			d.phrase = v;
		}

		void setServers(const QList<ServerSummary>& v)
		{
			d.servers = v;
		}

	private:
		class PrivData
		{
		public:
			QList<ServerSummary> servers;
			QString phrase;
		};
		PrivData d;
};

#endif
