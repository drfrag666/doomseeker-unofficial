//------------------------------------------------------------------------------
// wwwseeker.h
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
// Copyright (C) 2009 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef __WWWSEEKER_H__
#define __WWWSEEKER_H__

#include <QHash>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QString>
#include <QUrl>

class NetworkReplySignalWrapper;

/**
 *	@brief Search and protocol wrapper class.
 *
 *	WWWSeeker class seeks specified files by iterating through
 *	previously defined set of sites and parsing their HTML code
 *	if necessary.
 *
 *	This class handles URLs by selecting proper protocols for them.
 *	It hides how protocols work and provides simple interface:
 *	request a file, inform about download progress and
 *	send a proper signal when download is finished.
 *
 *	URLs with wildcards can be used, see: primaryFile
 */
class WWWSeeker : public QObject
{
	Q_OBJECT

	public:
		WWWSeeker();
		virtual ~WWWSeeker();

		/**
		 * @brief Adds a URL to a site where a specified filename may reside.
		 *
		 * Such site will be searched in a different order than global site
		 * URLs. Since it is suspected that these sites may contain specified
		 * files, they will have the priority.
		 *
		 * Naturally, such sites are also searched for all other seeked
		 * filenames, not the only specified one.
		 *
		 * If specified URL is on visited URLs list this becomes a no-op.
		 */
		void addFileSiteUrl(const QString& filename, const QUrl& url);

		/**
		 * @brief Adds a URL to a site that will be used in the search.
		 *
		 * URLs that were already used by this WWWSeeker object are rejected.
		 */
		void addSiteUrl(const QUrl& url);

		/**
		 * @brief Convenience method. Calls addSiteUrl() for each URL on The
		 * list.
		 *
		 * URLs that were already used by this WWWSeeker object are rejected.
		 */
		void addSitesUrls(const QList<QUrl>& urlsList);

		/**
		 * @brief Clears visited URLs list.
		 *
		 * This allows URLs to be reused by the current WWWSeeker object.
		 * Such URLs however need to be readded by using addSiteUrl() method.
		 */
		void clearVisitedUrlsList()
		{
			d.visitedUrls.clear();
		}

		/**
		 * @brief Checks if WWWSeeker is processing any data.
		 */
		bool isWorking() const
		{
			return d.bIsWorking;
		}

		int maxConcurrentSiteDownloads() const
		{
			return d.maxConcurrentSiteDownloads;
		}

		/**
		 * @brief Removes seeked filename from the current search.
		 *
		 * This filename will no longer be seeked on the downloaded sites.
		 *
		 * All sites that are suspected to refer to this filename will also
		 * be dropped.
		 *
		 * If all filenames are removed this is equal to abort().
		 *
		 * @param Case-insensitive filename to be removed. Only exact matches
		 * are dropped.
		 */
		void removeSeekedFilename(const QString& filename);

		/**
		 * @brief Maximum amount of URLs the WWWSeeker will go through
		 * at the same time.
		 */
		void setMaxConcurrectSiteDownloads(int value)
		{
			d.maxConcurrentSiteDownloads = value;
		}

		void setUserAgent(const QString& userAgent);

		/**
		 * @brief Begins search for files.
		 *
		 * Search is performed by downloading sites specified by sitesUrls
		 * list and seeking out links on these sites that either directly
		 * point to one of the specified filenames or may lead to subsites
		 * that may contain links to these files.
		 *
		 * WWWSeeker will look for all filenames specified on the list.
		 *
		 * This will do nothing if isWorking() returns true.
		 */
		void startSearch(const QStringList& seekedFilenames);

		const QString& userAgent() const;

	public slots:
		/**
		 * @brief Aborts the current search operation.
		 *
		 * finished() signal is emitted once the abort completes.
		 */
		void abort();

	signals:
		/**
		 * @brief Emitted when seeker aborts or there is nothing more to search.
		 */
		void finished();

		/**
		 * @brief Emitted for each direct link to a file that is found.
		 *
		 * @param filename
		 *      Filename to which the URL refers to.
		 * @param url
		 *      URL under which the file resides.
		 */
		void linkFound(const QString& filename, const QUrl& url);

		/**
		 * @brief Emitted when a WWW site finishes download.
		 */
		void siteFinished(const QUrl& site);

		/**
		 * @brief Emitted when a WWW site redirects to a different site.
		 */
		void siteRedirect(const QUrl& oldUrl, const QUrl& newUrl);

		/**
		 * @brief Emitted when a WWW site is being downloaded.
		 */
		void siteProgress(const QUrl& site, qint64 bytes, qint64 total);

		/**
		 * @brief Emitted when a download of a WWW site starts.
		 */
		void siteStarted(const QUrl& site);

	private:
		class NetworkQueryInfo
		{
			public:
				NetworkReplySignalWrapper* pSignalWrapper;
				QNetworkReply* pReply;

				NetworkQueryInfo(QNetworkReply* pReply);
				~NetworkQueryInfo();

				void deleteMembersLater();

				/**
				 * @brief NetworkQueryInfo objects are equal if their pReply
				 * is the same.
				 */
				bool operator==(const NetworkQueryInfo& other) const;
				bool operator!=(const NetworkQueryInfo& other) const
				{
					return ! (*this == other);
				}

				bool operator==(const QNetworkReply* pReply) const;
				bool operator!=(const QNetworkReply* pReply) const
				{
					return ! (*this == pReply);
				}
		};

		class PrivData
		{
			public:
				bool bIsAborting;
				bool bIsWorking;

				/**
				 * @brief URLs to sites where specified files may reside.
				 *
				 * Key - name of the file.
				 * Value - URLs to the site.
				 */
				QHash<QString, QList<QUrl> > fileSiteUrls;

				/**
				 * @brief Used to rotate over files on the fileSiteUrls
				 * hash map.
				 */
				QStringList fileSiteKeyRotationList;

				/**
				 * @brief Default value: 3
				 */
				int maxConcurrentSiteDownloads;

				/**
				 * @brief Currently running network queries.
				 *
				 * Amount here will not go above the maxConcurrentSiteDownloads.
				 */
				QList<NetworkQueryInfo*> networkQueries;

				QNetworkAccessManager* pNetworkAccessManager;

				QStringList seekedFilenames;

				/**
				 * @brief URLs that will be used in a search.
				 */
				QList<QUrl> sitesUrls;

				/**
				 * @brief Holds list of all used URLs.
				 *
				 * Prevents checking the same site twice.
				 */
				QList<QUrl> visitedUrls;

				/**
				 * @brief User Agent used in WWW queries.
				 */
				QString userAgent;
		};

		PrivData d;

		void addNetworkReply(QNetworkReply* pReply);
		void deleteNetworkQueryInfo(QNetworkReply* pReply);
		NetworkQueryInfo* findNetworkQueryInfo(QNetworkReply* pReply);
		bool isMoreToSearch() const;

		/**
		 * @brief Starts network query using specified URL.
		 *
		 * No limitation checks are performed here.
		 */
		void startNetworkQuery(const QUrl& url);

		/**
		 * @brief Takes next available URLs and starts queries to their sites.
		 *
		 * Limitation checks are performed here to make sure that given URL
		 * will not be visited twice and that not too many queries
		 * are running at once.
		 */
		void startNextSites();

		/**
		 * @brief Takes next non-visited site URL from the list and returns it.
		 *
		 * Taken URL is removed from the URLs list.
		 */
		QUrl takeNextUrl();
		bool wasUrlUsed(const QUrl& url) const;


	private slots:
		void networkQueryFinished(QNetworkReply* pReply);
};

#endif
