//------------------------------------------------------------------------------
// wadseekerwadstable.h
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
// Copyright (C) 2011 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef __WADSEEKERWADSTABLE_H__
#define __WADSEEKERWADSTABLE_H__

#include <QMap>
#include <QTableWidget>
#include <QTime>

class SpeedCalculator;

class WadseekerWadsTable : public QTableWidget
{
	Q_OBJECT;

	public:
		static const int IDX_NAME_COLUMN = 0;
		static const int IDX_URL_COLUMN = 1;
		static const int IDX_PROGRESS_COLUMN = 2;
		static const int IDX_SPEED_COLUMN = 3;
		static const int IDX_ETA_COLUMN = 4;
		static const int IDX_SIZE_COLUMN = 5;

		WadseekerWadsTable(QWidget* pParent = NULL);
		~WadseekerWadsTable();

		void addFile(const QString& filename);

	public slots:
		void setFileDownloadFinished(const QString& filename);
		void setFileProgress(const QString& filename, qint64 current, qint64 total);
		void setFileSuccessful(const QString& filename);
		void setFileUrl(const QString& filename, const QUrl& url);

	protected:
		void showEvent(QShowEvent* pEvent);

	private:
		class PrivData
		{
			public:
				bool bAlreadyShownOnce;

				/**
				 * @brief Hash Map containing SpeedCalculator objects for each
				 *        downloaded WAD.
				 *
				 * Key - filename
				 * Value - Pointer to SpeedCalculator instance.
				 */
				QMap<QString, SpeedCalculator* > speedCalculators;
				QTime updateClock;
		};

		static const int UPDATE_INTERVAL_MS = 1000;

		PrivData d;

		int findFileRow(const QString& filename);
		void updateDataInfoValues(bool bForce);
};

#endif
