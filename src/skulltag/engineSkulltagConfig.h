//------------------------------------------------------------------------------
// engineSkulltagConfig.h
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

#ifndef __ENGINE_SKULLTAG_CONFIG_H_
#define __ENGINE_SKULLTAG_CONFIG_H_

#include "gui/engineConfigBase.h"

class EngineSkulltagConfigBox : public BaseEngineConfigBox
{
	Q_OBJECT

	public:
		static ConfigurationBoxInfo* createStructure(const PluginInfo* plugin, IniSection* cfg, QWidget* parent = NULL);

		void readSettings();


	protected slots:
		void btnBrowseTestingPathClicked();

	protected:
		EngineSkulltagConfigBox(const PluginInfo* plugin, IniSection* cfg, QWidget* parent = NULL);
		void saveSettings();

		QGroupBox	*groupTesting;
		QLineEdit	*leTestingPath;
		QPushButton	*btnBrowseTestingPath;
};

#endif
