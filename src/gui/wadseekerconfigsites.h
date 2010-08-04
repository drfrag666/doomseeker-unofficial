//------------------------------------------------------------------------------
// wadseekerconfigsites.h
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
// Copyright (C) 2010 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#ifndef __WADSEEKERCONFIG_SITES_H_
#define __WADSEEKERCONFIG_SITES_H_

#include "ui_wadseekerconfigsites.h"
#include "gui/configBase.h"
#include <QKeyEvent>

class WadseekerSitesConfigBox : public ConfigurationBaseBox, private Ui::WadseekerSitesConfigBox
{
	Q_OBJECT

	public:
		static ConfigurationBoxInfo* createStructure(IniSection& cfg, QWidget* parent = NULL);

		void readSettings();

	protected slots:
		void btnUrlAddClicked();
		void btnUrlDefaultClicked();
		void btnUrlRemoveClicked();

		void focusChanged(QWidget* old, QWidget* now);

	protected:
		WadseekerSitesConfigBox(IniSection&, QWidget* parent = NULL);

		void 			saveSettings();
		void 			insertUrl(const QUrl& url);
		QStringList*	urlListEncoded();
};

#endif
