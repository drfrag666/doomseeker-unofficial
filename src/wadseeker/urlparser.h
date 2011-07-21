//------------------------------------------------------------------------------
// urlparser.h
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
#ifndef __URLPARSER_H__
#define __URLPARSER_H__

#include <QList>
#include <QStringList>
#include <QUrl>

class Link;

/**
 * @brief Parses URLs and splits them to groups of interest.
 *
 * This class operates on a list of URLs to divide them into groups.
 *
 * URLs are parsed to the Wadseeker needs. Direct links to files are extracted
 * as one group and links to subsites where the files may possibly reside
 * are extracted as the other group.
 */
class UrlParser
{
	public:
		UrlParser(const QList<Link>& links);

		/**
		 * @brief Extracts a list of URLs leading directly to any of the
		 *        wanted files.
		 *
		 * @param wantedFiles
		 *      List of files we wish to extract from the URLs specified
		 *      in the constructor. Any URL that matches any of the filenames
		 *      specified here will be matched.
		 * @param baseUrl
		 *      Relative URLs will be converted to absolute
		 *   	URLs using this URL as a base
		 *
		 * @return A list of URLs that match one or more of the filenames
		 *         on the wantedFiles list.
		 */
		QList<Link> directLinks(const QStringList& wantedFiles, const QUrl& baseUrl);

		/**
		 * @brief Extracts a list of URLs leading to subsites that may contain
		 *        any of the wanted files.
		 *
		 * @param wantedFiles
		 *      List of files we wish to extract from the URLs specified
		 *      in the constructor. Any URL that matches any of the filenames
		 *      specified here will be matched.
		 * @param baseUrl
		 *      Relative URLs will be converted to absolute
		 *   	URLs using this URL as a base
		 *
		 * @return A list of URLs that match one or more of the filenames
		 *         on the wantedFiles list.
		 */
		QList<Link> siteLinks(const QStringList& wantedFiles, const QUrl& baseUrl);

	private:
		class PrivData
		{
			public:
				QList<Link> links;
		};

		PrivData d;

		/**
		 * @brief Creates absolute URL by combining information derived from
		 *        relativeUrl and baseUrl.
		 *
		 * @b NOTE: If relativeUrl is already absolute, the returned URL will
		 * match the relativeUrl parameter.
		 *
		 * @return Absolute URL to a web resource. If a valid URL cannot be
		 * created a QUrl() object is returned.
		 */
		static QUrl createAbsoluteUrl(const QUrl& relativeUrl, const QUrl& baseUrl);

		static bool	hasFileReferenceSomewhere(const QStringList& wantedFileNames, const Link& link);
		static bool	isDirectLinkToFile(const QStringList& wantedFileNames, const Link& link);
};

#endif
