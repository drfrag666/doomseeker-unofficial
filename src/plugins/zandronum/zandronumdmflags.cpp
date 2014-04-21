//------------------------------------------------------------------------------
// zandronumdmflags.cpp
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
// Copyright (C) 2014 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#include "zandronumdmflags.h"

#include "serverapi/serverstructs.h"

DMFlagsSection ZandronumDmflags::compatflags()
{
	DMFlagsSection section("Compat. flags");
	section << DMFlag( tr("Use Doom's shortest texture behavior"),						0 );
	section << DMFlag( tr("Don't fix loop index for stair building"),						1 );
	section << DMFlag( tr("Pain elemental is limited to 20 lost souls"),					2 );
	section << DMFlag( tr("Pickups are only heard locally"),								3 );
	section << DMFlag( tr("Infinitly tall actors"),										4 );
	section << DMFlag( tr("Limit actors to only one sound"),								5 );
	section << DMFlag( tr("Enable wallrunning"),											6 );
	section << DMFlag( tr("Dropped items spawn on floor"),								7 );
	section << DMFlag( tr("Special lines block use line"),								8 );
	section << DMFlag( tr("Disable BOOM local door light effect"),						9 );
	section << DMFlag( tr("Raven's scrollers use their original speed"),					10 );
	section << DMFlag( tr("Use sector based sound target code"),							11 );
	section << DMFlag( tr("Limit dehacked MaxHealth to health bonus"),					12 );
	section << DMFlag( tr("Trace ignores lines with the same sector on both sides"),		13 );
	section << DMFlag( tr("Monsters can not move when hanging over a drop off"),			14 );
	section << DMFlag( tr("Scrolling sectors are additive like Boom"),					15 );
	section << DMFlag( tr("Monsters can see semi-invisible players"),						16 );
	section << DMFlag( tr("Limited movement in the air"),									17 );
	section << DMFlag( tr("Allow map01 \"plasma bump\" bug"),								18 );
	section << DMFlag( tr("Allow instant respawn after death"),							19 );
	section << DMFlag( tr("Disable taunting"),											20 );
	section << DMFlag( tr("Use doom2.exe's original sound curve"),						21 );
	section << DMFlag( tr("Use original doom2 intermission music"),						22 );
	section << DMFlag( tr("Disable stealth monsters"),									23 );
	section << DMFlag( tr("Disable crosshair"),											25 );
	section << DMFlag( tr("Force weapon switch"),											26 );
	section << DMFlag( tr("Instantly moving floors are not silent"),						28 );
	section << DMFlag( tr("Sector sounds use original method for sound orgin"),			29 );
	section << DMFlag( tr("Use original Doom heights for clipping against projetiles"),	30 );
	section << DMFlag( tr("Monsters can't be pushed over drop offs"),						31 );
	return section;
}

DMFlagsSection ZandronumDmflags::dmflags()
{
	DMFlagsSection section("DMFlags");
	section << DMFlag( tr("Do not spawn health items (DM)"),				0 );
	section << DMFlag( tr("Do not spawn powerups (DM)"), 					1 );
	section << DMFlag( tr("Weapons remain after pickup (DM)"),				2 );
	section << DMFlag( tr("Falling damage (ZDoom/Strife)"),					3 );
	section << DMFlag( tr("Falling damage (Hexen/Strife)"), 				4 );
	section << DMFlag( tr("Stay on same map when someone exits (DM)"),		6 );
	section << DMFlag( tr("Spawn players as far as possible (DM)"),			7 );
	section << DMFlag( tr("Automatically respawn dead players (DM)"),		8 );
	section << DMFlag( tr("Don't spawn armor (DM)"),						9 );
	section << DMFlag( tr("Kill anyone who tries to exit the level (DM)"),	10 );
	section << DMFlag( tr("Infinite ammo"),									11 );
	section << DMFlag( tr("No monsters"),									12 );
	section << DMFlag( tr("Monsters respawn"),								13 );
	section << DMFlag( tr("Items other than invuln. and invis. respawn"),	14 );
	section << DMFlag( tr("Fast monsters"),									15 );
	section << DMFlag( tr("No jumping"),									16 );
	section << DMFlag( tr("No freelook"),									17 );
	section << DMFlag( tr("Respawn invulnerability and invisibility"),		18 );
	section << DMFlag( tr("Arbitrator FOV"),								19 );
	section << DMFlag( tr("No multiplayer weapons in cooperative"),			20 );
	section << DMFlag( tr("No crouching"),									21 );
	section << DMFlag( tr("Lose all old inventory on respawn (COOP)"),		22 );
	section << DMFlag( tr("Lose keys on respawn (COOP)"),					23 );
	section << DMFlag( tr("Lose weapons on respawn (COOP)"),				24 );
	section << DMFlag( tr("Lose armor on respawn (COOP)"),					25 );
	section << DMFlag( tr("Lose powerups on respawn (COOP)"),				26 );
	section << DMFlag( tr("Lose ammo on respawn (COOP)"),					27 );
	section << DMFlag( tr("Lose half your ammo on respawn (COOP)"),			28 );
	section << DMFlag( tr("Jumping allowed"),								29 );
	section << DMFlag( tr("Crouching allowed"),								30 );
	return section;
}

DMFlagsSection ZandronumDmflags::dmflags2()
{
	DMFlagsSection section("DMFlags2");
	section << DMFlag( tr("Drop weapons upon death"), 						1 );
	section << DMFlag( tr("Don't spawn runes"),								2 );
	section << DMFlag( tr("Instantly return flags (ST/CTF)"),					3 );
	section << DMFlag( tr("Don't allow players to switch teams"),				4 );
	section << DMFlag( tr("Players are automatically assigned teams"),		5 );
	section << DMFlag( tr("Double the amount of ammo given"),					6 );
	section << DMFlag( tr("Players slowly lose health over 100% like Quake"),	7 );
	section << DMFlag( tr("Allow BFG freeaiming"),							8 );
	section << DMFlag( tr("Barrels respawn"),									9 );
	section << DMFlag( tr("No respawn protection"),							10 );
	section << DMFlag( tr("All players start with a shotgun"),				11 );
	section << DMFlag( tr("Players respawn where they died (COOP)"),			12 );
	section << DMFlag( tr("Players keep teams after map change"),				13 );
	section << DMFlag( tr("Don't clear frags after each level"),				14 );
	section << DMFlag( tr("Player can't respawn"),							15 );
	section << DMFlag( tr("Lose a frag when killed"),							16 );
	section << DMFlag( tr("Infinite inventory"),								17 );
	section << DMFlag( tr("No rocket jumping"),								19 );
	section << DMFlag( tr("Award damage not kills"),							20 );
	section << DMFlag( tr("Force drawing alpha"),								21 );
	section << DMFlag( tr("All monsters must be killed before exiting"),		22 );
	section << DMFlag( tr("Players can't see the automap"),					23 );
	section << DMFlag( tr("Allies can't be seen on the automap"),				24 );
	section << DMFlag( tr("You can't spy allies"),							25 );
	section << DMFlag( tr("Players can use chase cam"),						26 );
	section << DMFlag( tr("Players can suicide"),								27 );
	section << DMFlag( tr("Players can not use autoaim"),						28 );
	return section;
}

///////////////////////////////////////////////////////////////////////////////

DMFlagsSection ZandronumDmflags1point0::compatflags2()
{
	DMFlagsSection section("Compat. flags 2");
	section << DMFlag(tr("NETSCRIPTS are clientside"), 0);
	section << DMFlag(tr("Clients send full button info"), 1);
	section << DMFlag(tr("Disallow 'land' console command"), 2);
	section << DMFlag(tr("Old random generator"), 3);
	section << DMFlag(tr("Powerup spheres have NOGRAVITY flag"), 4);
	section << DMFlag(tr("Don't stop scripts running on a player when player leaves the server"), 5);
	section << DMFlag(tr("Explosion causes strong horizontal thrust (like old ZDoom)"), 6);
	section << DMFlag(tr("Non-SOLID things fall through thing bridges"), 7);
	section << DMFlag(tr("ZDoom 123B33 jump physics"), 8);
	section << DMFlag(tr("Can't change weapons during raise/lower"), 9);
	return section;
}

DMFlagsSection ZandronumDmflags1point0::dmflags3()
{
	DMFlagsSection section("DMFlags3");
	section << DMFlag(tr("Don't display names of targeted players"), 0);
	section << DMFlag(tr("Apply lmsspectatorsettings in all game modes"), 1);
	section << DMFlag(tr("Force no coop info"), 2);
	section << DMFlag(tr("No unlagged"), 3);
	section << DMFlag(tr("Noclip through other players"), 4);
	section << DMFlag(tr("No medals"), 5);
	return section;
}

QList<DMFlagsSection> ZandronumDmflags1point0::flags()
{
	ZandronumDmflags common;
	QList<DMFlagsSection> result;
	result << common.dmflags();
	result << common.dmflags2();
	result << dmflags3();
	result << common.compatflags();
	result << compatflags2();
	return result;
}