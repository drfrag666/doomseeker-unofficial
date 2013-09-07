//------------------------------------------------------------------------------
// serverfilterbuildermenu.cpp
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
// Copyright (C) 2013 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#include "serverfilterbuildermenu.h"

#include "gui/entity/serverlistfilterinfo.h"
#include "serverapi/server.h"

class ServerFilterBuilderMenu::PrivData
{
	public:
		ServerListFilterInfo filter;

		static void addIfNotContains(QStringList& target, const QString& candidate)
		{
			if (!target.contains(candidate, Qt::CaseInsensitive))
			{
				target << candidate;
			}
		}
};

ServerFilterBuilderMenu::ServerFilterBuilderMenu(const Server& server,
	const ServerListFilterInfo& filter, QWidget* parent)
: QMenu(tr("Build server filter ..."), parent)
{
	d = new PrivData();
	d->filter = filter;

	QMenu* includeWads = new QMenu(tr("Include WAD ..."), this);
	QMenu* excludeWads = new QMenu(tr("Exclude WAD ..."), this);

	QStringList wads;
	if (!server.iwadName().trimmed().isEmpty())
	{
		wads << server.iwadName();
	}
	foreach (const PWad& wad, server.pwads())
	{
		wads << wad.name;
	}
	foreach (const QString& wad, wads)
	{
		if (!d->filter.wadsExcluded.contains(wad, Qt::CaseInsensitive))
		{
			mkExcludeWadAction(excludeWads, wad);
		}
		if (!d->filter.wads.contains(wad, Qt::CaseInsensitive))
		{
			mkIncludeWadAction(includeWads, wad);
		}
	}

	if (!excludeWads->isEmpty())
	{
		addMenu(excludeWads);
	}
	if (!includeWads->isEmpty())
	{
		addMenu(includeWads);
	}
}

ServerFilterBuilderMenu::~ServerFilterBuilderMenu()
{
	delete d;
}

void ServerFilterBuilderMenu::excludeWadFromAction()
{
	QAction* action = static_cast<QAction*>(sender());
	PrivData::addIfNotContains(d->filter.wadsExcluded, action->text());
}

const ServerListFilterInfo& ServerFilterBuilderMenu::filter() const
{
	return d->filter;
}

void ServerFilterBuilderMenu::includeWadFromAction()
{
	QAction* action = static_cast<QAction*>(sender());
	PrivData::addIfNotContains(d->filter.wads, action->text());
}

QAction* ServerFilterBuilderMenu::mkExcludeWadAction(QMenu* parent, const QString& wadName)
{
	QAction* action = new QAction(parent);
	action->setText(wadName);
	this->connect(action, SIGNAL(triggered()), SLOT(excludeWadFromAction()));
	parent->addAction(action);
	return action;
}

QAction* ServerFilterBuilderMenu::mkIncludeWadAction(QMenu* parent, const QString& wadName)
{
	QAction* action = new QAction(parent);
	action->setText(wadName);
	this->connect(action, SIGNAL(triggered()), SLOT(includeWadFromAction()));
	parent->addAction(action);
	return action;
}
