//------------------------------------------------------------------------------
// http.h
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
// Copyright (C) 2009 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef __HTTP_H_
#define __HTTP_H_

#include "link.h"
#include <QDebug>
#include <QFileInfo>
#include <QHttp>
#include <QHttpRequestHeader>
#include <QHttpResponseHeader>
#include <QList>
#include <QStringList>
#include <Qt>
#include <QUrl>

class Http : public QObject
{
	Q_OBJECT

	public:
		enum STATUS_CODES
		{
			STATUS_OK 		= 200,
			STATUS_REDIRECT = 302
		};

		enum HTTP_FILE_TYPE
		{
			HTTP_FILE_TYPE_BINARY 	= 0,
			HTTP_FILE_TYPE_HTML 	= 1,
			HTTP_FILE_TYPE_UNKNOWN 	= 2
		};

		Http();
		Http(QString);
		~Http();

		void abort();

		bool				isBinaryFile(const QFileInfo&);
		bool				isHTMLFile(const QFileInfo&);
		QByteArray&			lastData() { return data; }
		HTTP_FILE_TYPE		lastFileType() { return fileType; }
		int					lastResponseCode() const { return responseCode; }
		const QString&		lastResponsePhrase() const { return responsePhrase; }
		QUrl				lastLink() const;
		QList<Link>			links();
		void 				sendRequestGet(QString);
		void				setBinaryFilesExtensions(const QStringList& list);
		void 				setSite(const QString&);

	signals:
		void dataReceived(unsigned howMuch, unsigned howMuchSum, unsigned percent);
		void error(const QString&);
		void finishedReceiving(const QString& error);
		void size(unsigned int);

	protected slots:
		void done(bool);
		void headerReceived(const QHttpResponseHeader&);
		void read(const QHttpResponseHeader&);

	protected:
		static QStringList		htmlFileExtensions;

		QStringList				binaryFileExtensions;
		QByteArray				data;
		bool					dontSendFinishedReceiving;
		HTTP_FILE_TYPE			fileType;
		QHttp 					qHttp;
		int						responseCode;
		QString					responsePhrase;
		QString					resource;
		QString 				site;
		unsigned 				sizeCur;
		unsigned				sizeMax;

		/**
		 * Capitalizes all keywords
		 */
		void					capitalizeTags(QByteArray&);

		/**
		 * Finds a HTML tag starting from index in the byte array.
		 * @param byte		- array that will be searched
		 * @param beginAt 	- index from which searching starts
		 * @param end 		- end index of a tag
		 * @return 			- begin index of a tag
		 */
		int						findTag(QByteArray& byte, int beginAt, int* end);

		/**
		 * You put something like HREF="http://127.0.0.1" and it retrieves the thing after '=' without the "".
		 * @param byte			- array that will be searched
		 * @param beginIndex 	- index from which parsing starts
		 * @param endIndex		- index at which parsing ends
		 * @return 				- trimmed value, without white-spaces and quotes.
		 */
		QString					htmlValue(QByteArray& byte, int beginIndex, int endIndex);

	private:
		void					construct();
};

#endif
