//------------------------------------------------------------------------------
// ircresponseparseresult.h
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
// Copyright (C) 2011 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef __IRCRESPONSEPARSERESULT_H__
#define __IRCRESPONSEPARSERESULT_H__

#include <cstdlib>

class IRCResponseType;

/**
 * @brief Result info generated by the IRCResponseParser.
 */
class IRCResponseParseResult
{
public:
	/**
	 * @brief Creates an invalid object.
	 *
	 * Invalid object means that error occured and the parsing didn't
	 * even occur.
	 */
	IRCResponseParseResult();

	/**
	 * @brief A valid response result with set type and notification if
	 *        it was parsed or not.
	 *
	 * @param responseType
	 *      Type of the parsed response. Please note that this may still
	 *      be invalid, even if the entire result object is valid.
	 * @param bWasParsed
	 *      <code>true</code> if IRCResponseParser attempted to parse
	 *      the message. <code>false</code> if message was completely
	 *      ignored by the parser.
	 */
	IRCResponseParseResult(const IRCResponseType &responseType, bool bWasParsed);

	IRCResponseParseResult(const IRCResponseParseResult &other);
	~IRCResponseParseResult();

	bool isValid() const
	{
		return d.pResponseType != nullptr;
	}

	IRCResponseParseResult &operator=(const IRCResponseParseResult &other);

	/**
	 * @brief Type of the response as defined by IRCResponseType.
	 *
	 * If isValid() returns <code>false</code> this will also return an
	 * invalid IRCResponseType object.
	 */
	const IRCResponseType &type() const;

	/**
	 * @brief <code>true</code> if response message was parsed,
	 *        <code>false</code> if IRCResponseParser ignored the response.
	 */
	bool wasParsed() const
	{
		return d.bWasParsed;
	}

private:
	class PrivData
	{
	public:
		bool bWasParsed;
		IRCResponseType *pResponseType;
	};

	PrivData d;

	void copyIn(const IRCResponseParseResult &other);
};

#endif
