//------------------------------------------------------------------------------
// zandronumgamehost.cpp
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
#include "zandronumgamehost.h"
#include "zandronumengineplugin.h"
#include "zandronumgamerunner.h"
#include "zandronumgameinfo.h"
#include "zandronumserver.h"
#include <serverapi/gamerunnerstructs.h>

ZandronumGameHost::ZandronumGameHost(const ZandronumServer* server)
: GameHost(server)
{
	this->server = server;
	setArgForServerLaunch("-host");

	set_hostDMFlags(&ZandronumGameHost::hostDMFlags);
}

void ZandronumGameHost::hostDMFlags()
{
	QStringList& args = cmdLine()->args;
	DMFlags dmFlags = hostInfo().dmFlags;
	const QString argNames[] = { "+dmflags", "+dmflags2", "+compatflags" };
	for (int i = 0; i < qMin(dmFlags.size(), 3); ++i)
	{
		unsigned flagsValue = 0;
		const DMFlagsSection* section = dmFlags[i];

		for (int j = 0; j < section->flags.count(); ++j)
		{
			flagsValue |= 1 << section->flags[j].value;
		}

		args << argNames[i] << QString::number(flagsValue);
	}
}

void ZandronumGameHost::hostProperties(QStringList& args) const
{
	args << "+alwaysapplydmflags" << QString::number(1);
	args << "-skill" << QString::number(server->skill() + 1); // from 1 to 5

	QString gameModeStr;
	switch(server->gameMode().modeIndex())
	{
		case GameMode::SGMICooperative:						gameModeStr = "+cooperative"; break;
		case GameMode::SGMICTF:								gameModeStr = "+ctf"; break;
		case GameMode::SGMIDeathmatch:						gameModeStr = "+deathmatch"; break;
		case GameMode::SGMITeamDeathmatch:					gameModeStr = "+teamplay"; break;
		case ZandronumGameInfo::GAMEMODE_DOMINATION:		gameModeStr = "+domination"; break;
		case ZandronumGameInfo::GAMEMODE_DUEL:				gameModeStr = "+duel"; break;
		case ZandronumGameInfo::GAMEMODE_INVASION:			gameModeStr = "+invasion"; break;
		case ZandronumGameInfo::GAMEMODE_LASTMANSTANDING:	gameModeStr = "+lastmanstanding"; break;
		case ZandronumGameInfo::GAMEMODE_ONEFLAGCTF:		gameModeStr = "+oneflagctf"; break;
		case ZandronumGameInfo::GAMEMODE_POSSESSION:		gameModeStr = "+possession"; break;
		case ZandronumGameInfo::GAMEMODE_SKULLTAG:			gameModeStr = "+skulltag"; break;
		case ZandronumGameInfo::GAMEMODE_SURVIVAL:			gameModeStr = "+survival"; break;
		case ZandronumGameInfo::GAMEMODE_TEAMGAME:			gameModeStr = "+teamgame"; break;
		case ZandronumGameInfo::GAMEMODE_TEAMLMS:			gameModeStr = "+teamlms"; break;
		case ZandronumGameInfo::GAMEMODE_TEAMPOSSESSION:	gameModeStr = "+teampossession"; break;
		case ZandronumGameInfo::GAMEMODE_TERMINATOR:		gameModeStr = "+terminator"; break;
	}
	args << gameModeStr << "1";

	args << "+sv_hostemail" << "\"" + server->email() + "\"";

	if (!server->map().isEmpty())
	{
		args << "+map" << server->map();
	}

	const QStringList& mapsList = server->mapList();
	if (!mapsList.isEmpty())
	{
		foreach (QString map, mapsList)
		{
			args << "+addmap" << map;
		}
	}

	args << "+sv_maprotation" << QString::number(
		static_cast<int>(!mapsList.isEmpty()));
	args << "+sv_randommaprotation" << QString::number(
		static_cast<int>(server->isRandomMapRotation()) );

	QString motd = server->motd();
	args << "+sv_motd" << "\"" + motd.replace("\n", "\\n") + "\"";

	args << "+sv_hostname" << "\"" + server->name() + "\"";

	args << "+sv_website" << "\"" + server->webSite() + "\"";

	QString password = server->connectPassword();
	args << "+sv_password" << "\"" + password + "\"";
	args << "+sv_forcepassword" << QString::number(static_cast<int>(!password.isEmpty()));

	password = server->joinPassword();
	args << "+sv_joinpassword" << "\"" + password + "\"";
	args << "+sv_forcejoinpassword" << QString::number(static_cast<int>(!password.isEmpty()));

	password = server->rconPassword();
	args << "+sv_rconpassword" << "\"" + password + "\"";

	args << "+sv_broadcast" << QString::number(static_cast<int>( server->isBroadcastToLAN() ));
	args << "+sv_updatemaster" << QString::number(static_cast<int>( server->isBroadcastToMaster() ));
	args << "+sv_maxclients" << QString::number(server->numTotalSlots());
	args << "+sv_maxplayers" << QString::number(server->maxPlayers());
}
