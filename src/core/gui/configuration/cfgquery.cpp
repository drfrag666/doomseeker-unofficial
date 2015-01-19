//------------------------------------------------------------------------------
// cfgquery.cpp
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
// Copyright (C) 2009 "Blzut3" <admin@maniacsvault.net>
//------------------------------------------------------------------------------
#include "cfgquery.h"
#include "ui_cfgquery.h"
#include "configuration/doomseekerconfig.h"
#include "configuration/queryspeed.h"

class CFGQuery::PrivData : public Ui::CFGQuery
{
public:
	void setQuerySpeed(const QuerySpeed &speed)
	{
		triesBox->setValue(speed.attemptsPerServer);
		timeoutBox->setValue(speed.delayBetweenSingleServerAttempts);
		queryIntervalBox->setValue(speed.intervalBetweenServers);
	}
	
	QuerySpeed querySpeed() const
	{
		QuerySpeed speed;
		speed.attemptsPerServer = triesBox->value();
		speed.delayBetweenSingleServerAttempts = timeoutBox->value();
		speed.intervalBetweenServers = queryIntervalBox->value();
		return speed;
	}
};

CFGQuery::CFGQuery(QWidget *parent)
: ConfigurationBaseBox(parent)
{
	d = new PrivData;
	d->setupUi(this);
}

CFGQuery::~CFGQuery()
{
	delete d;
}

void CFGQuery::readSettings()
{
	d->queryBeforeLaunch->setChecked(gConfig.doomseeker.bQueryBeforeLaunch);
	d->queryOnStartup->setChecked(gConfig.doomseeker.bQueryOnStartup);
	d->grbServerAutoRefresh->setChecked(gConfig.doomseeker.bQueryAutoRefreshEnabled);
	d->numAutoRefreshEverySeconds->setValue(gConfig.doomseeker.queryAutoRefreshEverySeconds);
	d->cbDontRefreshIfActive->setChecked(gConfig.doomseeker.bQueryAutoRefreshDontIfActive);
	d->setQuerySpeed(gConfig.doomseeker.querySpeed());
}

void CFGQuery::saveSettings()
{
	gConfig.doomseeker.bQueryBeforeLaunch = d->queryBeforeLaunch->isChecked();
	gConfig.doomseeker.bQueryOnStartup = d->queryOnStartup->isChecked();
	gConfig.doomseeker.bQueryAutoRefreshEnabled = d->grbServerAutoRefresh->isChecked();
	gConfig.doomseeker.queryAutoRefreshEverySeconds = d->numAutoRefreshEverySeconds->value();
	gConfig.doomseeker.bQueryAutoRefreshDontIfActive = d->cbDontRefreshIfActive->isChecked();
	gConfig.doomseeker.setQuerySpeed(d->querySpeed());
}

void CFGQuery::setCautiousQueryPreset()
{
	d->setQuerySpeed(QuerySpeed::cautious());
}

void CFGQuery::setModerateQueryPreset()
{
	d->setQuerySpeed(QuerySpeed::moderate());
}

void CFGQuery::setAggressiveQueryPreset()
{
	d->setQuerySpeed(QuerySpeed::aggressive());
}
