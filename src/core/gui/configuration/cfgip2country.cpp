//------------------------------------------------------------------------------
// cfgip2c.cpp
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
#include "cfgip2country.h"
#include "ui_cfgip2country.h"
#include "configuration/doomseekerconfig.h"

DClass<CFGIP2Country> : public Ui::CFGIP2Country
{
};

DPointered(CFGIP2Country)

CFGIP2Country::CFGIP2Country(QWidget *parent)
: ConfigurationBaseBox(parent)
{
	d->setupUi(this);
}

CFGIP2Country::~CFGIP2Country()
{
}

void CFGIP2Country::readSettings()
{
	d->leIP2CUrl->setText(gConfig.doomseeker.ip2CountryUrl);
	d->grbAutoUpdate->setChecked(gConfig.doomseeker.bIP2CountryAutoUpdate);
	d->spinMaximumAge->setValue(gConfig.doomseeker.ip2CountryDatabaseMaximumAge);
}

void CFGIP2Country::saveSettings()
{
	gConfig.doomseeker.ip2CountryUrl = d->leIP2CUrl->text();
	gConfig.doomseeker.bIP2CountryAutoUpdate = d->grbAutoUpdate->isChecked();
	gConfig.doomseeker.ip2CountryDatabaseMaximumAge = d->spinMaximumAge->value();
}
