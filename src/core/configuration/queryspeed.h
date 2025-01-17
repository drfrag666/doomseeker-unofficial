//------------------------------------------------------------------------------
// queryspeed.h
//------------------------------------------------------------------------------
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301  USA
//
//------------------------------------------------------------------------------
// Copyright (C) 2015 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef idC8DC23B5_9EE8_4248_A438B00780F217CA
#define idC8DC23B5_9EE8_4248_A438B00780F217CA

struct QuerySpeed
{
	static const QuerySpeed MAX_SPEED;
	static const int MAX_ATTEMPTS_PER_SERVER;

	static QuerySpeed cautious();
	static QuerySpeed moderate();
	static QuerySpeed aggressive();
	static QuerySpeed veryAggressive();

	int attemptsPerServer;
	int delayBetweenSingleServerAttempts;
	int intervalBetweenServers;
};

#endif
