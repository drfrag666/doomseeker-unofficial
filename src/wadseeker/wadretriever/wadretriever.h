//------------------------------------------------------------------------------
// wadretriever.h
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
#ifndef __WADRETRIEVER_H__
#define __WADRETRIEVER_H__

#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>

class NetworkReplyWrapperInfo;
class WadDownloadInfo;

/**
 * @brief Retrieves WADs from remote locations and install them in a save
 *        directory.
 *
 * Multiple WADs are downloaded in parallel network requests. The limit of
 * network requests that may be running at once is specified by
 * setMaxConcurrentWadDownloads() method.
 *
 * Once a file is retrieved successfully and matches a name of one of the
 * WADs specified on a list set through setWads() method, this file is stored in
 * path specified through setTargetSavePath() method.
 *
 * If a file that is retrieved from a remote location is identified as an
 * archive, WadRetriever will go through this file in search for all WADs
 * specified for download.
 *
 * For each WAD that is installed successfully the wadInstalled() signal is
 * emitted and the WAD is removed from the download list.
 */
class WadRetriever : public QObject
{
	Q_OBJECT

	public:
		WadRetriever();

		/**
		 * @brief Provides new URL for a WAD download.
		 *
		 * This will start download if necessary. Please note that only one
		 * download of a single WAD may be running at single time. However
		 * multiple different WADs may be downloaded concurrently.
		 *
		 * @param wad
		 *      WAD for which the URL will be added. If WAD is not on the list
		 *      set by setWads() the URL will not be added.
		 * @param url
		 *      Download URL that will be queued for specified WAD.
		 */
		void addUrl(const WadDownloadInfo& wad, const QUrl& url);

		/**
		 * @brief Checks if WadRetriever has specified WAD on the list.
		 */
		bool hasWad(const WadDownloadInfo& wad) const;

		int numCurrentRunningDownloads() const;

		/**
		 * @brief Max concurrent downloads of different WADs.
		 *
		 * Default value: 3.
		 */
		void setMaxConcurrentWadDownloads(int value)
		{
			d.maxConcurrentWadDownloads = value;
		}

		/**
		 * @brief Directory where the installed WADs will be saved.
		 */
		void setTargetSavePath(const QString& path)
		{
			d.targetSavePath = path;
		}

		/**
		 * @brief Sets a list of WADs that will be retrieved.
		 *
		 * This will not start the download by itself. Download URLs must be
		 * provided first
		 *
		 * @param wads
		 *      List of WadDownloadInfo objects describing WADs that will be
		 *      downloaded and installed. Contents of this list are copied.
		 */
		void setWads(const QList<WadDownloadInfo>& wads);

		/**
		 * @brief Directory where the installed WADs will be saved.
		 */
		const QString& targetSavePath() const
		{
			return d.targetSavePath;
		}

		/**
		 * @brief Attempt to install specified file as a downloaded WAD.
		 *
		 * This omits the download procedure treating data param as already
		 * downloaded file contents. If data is recognized as an archive
		 * it will be
		 */
		void tryInstall(const QString& filename, const QByteArray& data);

	signals:
		/**
		 * @brief Emitted when a WAD is successfully installed.
		 *
		 * @param wad
		 *      Instance of WadDownloadInfo describing the WAD that was
		 *      installed.
		 */
		void wadInstalled(const WadDownloadInfo& wad);

	private:
		class WadRetrieverInfo
		{
			public:
				QList<QUrl> downloadUrls;
				NetworkReplyWrapperInfo* pNetworkReply;
				WadDownloadInfo* wad;

				WadRetrieverInfo(const WadDownloadInfo& wad);
				~WadRetrieverInfo();

				bool operator==(const WadDownloadInfo& wad) const;
				bool operator!=(const WadDownloadInfo& wad) const;

				/**
				 * @brief Objects are equal if WadDownloadInfo objects are
				 *        equal.
				 */
				bool operator==(const WadRetrieverInfo& other) const;
				bool operator!=(const WadRetrieverInfo& other) const;
		};

		class PrivData
		{
			public:
				int maxConcurrentWadDownloads;
				QNetworkAccessManager networkAccessManager;
				QString targetSavePath;
				QList<WadRetrieverInfo> wads;
		};

		PrivData d;

		WadRetrieverInfo* findRetrieverInfo(const WadDownloadInfo& wad);
		WadRetrieverInfo* findRetrieverInfo(const QString& wadName);
		WadRetrieverInfo* findRetrieverInfo(const QNetworkReply* pNetworkReply);
		void startNextDownloads();
};

#endif
