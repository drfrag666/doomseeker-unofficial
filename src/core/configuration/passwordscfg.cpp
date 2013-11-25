//------------------------------------------------------------------------------
// passwordscfg.cpp
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
#include "passwordscfg.h"

#include "configuration/doomseekerconfig.h"
#include "configuration/serverpassword.h"
#include "ini/ini.h"
#include "ini/inisection.h"
#include "ini/inivariable.h"
#include "serverapi/server.h"
#include <cassert>
#include <QDebug>

const QString SECTION_NAME = "Passwords";

const QString MAX_NUMBER_OF_SERVERS_PER_PASSWORD_KEY = "MaxNumberOfServersPerPassword";
const QString REMEMBER_CONNECT_PASSWORD = "RememberConnectPassword";
const QString SERVER_PASSWORDS_KEY = "ServerPasswords";

// TODO: [Zalewa] We have 3 different ini files, all instantiated
// in Main and kept in different places. Perhaps we should move them
// out to a separate singleton? If not, then perhaps we could at least
// move the instantiation out of Main.
Ini* PasswordsCfg::ini = NULL;


bool serverDateDescending(ServerPassword::Server& s1, ServerPassword::Server& s2)
{
	return s1.time() > s2.time();
}


class PasswordsCfg::PrivData
{
	public:
		IniSection section;
};

PasswordsCfg::PasswordsCfg()
{
	assert(ini != NULL && "instantiated PasswordsCfg() without initing ini");
	d = new PrivData();
	d->section = ini->section(SECTION_NAME);
}

PasswordsCfg::~PasswordsCfg()
{
	delete d;
}

void PasswordsCfg::cutServers(QList<ServerPassword>& passwords) const
{
	QMutableListIterator<ServerPassword> it(passwords);
	while (it.hasNext())
	{
		ServerPassword& password = it.next();
		QList<ServerPassword::Server> sortedServers = password.servers();
		qSort(sortedServers.begin(), sortedServers.end(), serverDateDescending);
		password.setServers(sortedServers.mid(0, maxNumberOfServersPerPassword()));
	}
}

void PasswordsCfg::cutStoredServers()
{
	QList<ServerPassword> passwords = serverPasswords();
	cutServers(passwords);
	storeServerPasswords(passwords);
}

void PasswordsCfg::initIni(const QString& path)
{
	assert(ini == NULL && "tried to re-init password ini");
	if (ini != NULL)
	{
		qDebug() << "Error: tried to re-init password ini";
		return;
	}
	ini = new Ini(path);
}

bool PasswordsCfg::isHidingPasswords() const
{
	return gConfig.doomseeker.bHidePasswords;
}

bool PasswordsCfg::isRememberingConnectPhrase() const
{
	return d->section.value(REMEMBER_CONNECT_PASSWORD, false).toBool();
}

int PasswordsCfg::maxNumberOfServersPerPassword() const
{
	return d->section.value(MAX_NUMBER_OF_SERVERS_PER_PASSWORD_KEY, 5).toInt();
}

void PasswordsCfg::removeServerPhrase(const QString& phrase)
{
	QList<ServerPassword> allPasswords = serverPasswords();
	QMutableListIterator<ServerPassword> it(allPasswords);
	while (it.hasNext())
	{
		ServerPassword existingPass = it.next();
		if (existingPass.phrase() == phrase)
		{
			it.remove();
		}
	}
	storeServerPasswords(allPasswords);
}

void PasswordsCfg::saveServerPhrase(const QString& phrase, const Server* server)
{
	ServerPassword::Server serverInfo;
	if (server != NULL)
	{
		serverInfo.setGame(server->engineName());
		serverInfo.setAddress(server->address().toString());
		serverInfo.setPort(server->port());
		serverInfo.setName(server->name());
		serverInfo.setTime(QDateTime::currentDateTime());
	}
	QList<ServerPassword> allPasswords = serverPasswords();
	QMutableListIterator<ServerPassword> it(allPasswords);
	while (it.hasNext())
	{
		// Try to add server to existing password.
		ServerPassword& existingPass = it.next();
		if (existingPass.phrase() == phrase)
		{
			if (serverInfo.isValid())
			{
				existingPass.addServer(serverInfo);
			}
			setServerPasswords(allPasswords);
			return;
		}
	}
	// Add new.
	ServerPassword pass;
	pass.setPhrase(phrase);
	pass.addServer(serverInfo);
	allPasswords << pass;
	storeServerPasswords(allPasswords);
}

void PasswordsCfg::setHidePasswords(bool val)
{
	gConfig.doomseeker.bHidePasswords = val;
}

QList<ServerPassword> PasswordsCfg::serverPasswords() const
{
	QList<ServerPassword> result;
	QVariantList vars = d->section[SERVER_PASSWORDS_KEY].value().toList();
	foreach (const QVariant& var, vars)
	{
		result << ServerPassword::deserializeQVariant(var);
	}
	return result;
}

QStringList PasswordsCfg::serverPhrases() const
{
	QStringList result;
	foreach (const ServerPassword& pass, serverPasswords())
	{
		result << pass.phrase();
	}
	return result;
}

void PasswordsCfg::setMaxNumberOfServersPerPassword(int val)
{
	bool shallCut = val < maxNumberOfServersPerPassword();
	d->section.setValue(MAX_NUMBER_OF_SERVERS_PER_PASSWORD_KEY, val);
	if (shallCut)
	{
		cutStoredServers();
	}
}

void PasswordsCfg::setRememberConnectPhrase(bool val)
{
	return d->section.setValue(REMEMBER_CONNECT_PASSWORD, val);
}

void PasswordsCfg::setServerPasswords(const QList<ServerPassword>& val)
{
	QList<ServerPassword> passwords = val;
	cutServers(passwords);
	storeServerPasswords(passwords);
}

void PasswordsCfg::storeServerPasswords(const QList<ServerPassword>& val)
{
	QVariantList vars;
	foreach (const ServerPassword obj, val)
	{
		vars << obj.serializeQVariant();
	}
	d->section.setValue(SERVER_PASSWORDS_KEY, vars);
}

ServerPassword PasswordsCfg::suggestPassword(const Server* server)
{
	// This method would probably work better as a separate class.
	// If there's ever any need to expand it, extract it
	// to a PasswordSuggester or something like that first.
	ServerPassword::Server serverSummary;
	serverSummary.setAddress(server->address().toString());
	serverSummary.setPort(server->port());
	serverSummary.setGame(server->engineName());
	serverSummary.setName(server->name());

	ServerPassword password;
	ServerPassword::Server bestFit;
	foreach (const ServerPassword& potentialPassword, serverPasswords())
	{
		float newSimilarity;
		ServerPassword::Server candidate = potentialPassword.mostSimilarServer(serverSummary, &newSimilarity);
		if (candidate.isValid())
		{
			if (newSimilarity > bestFit.similarity(serverSummary))
			{
				bestFit = candidate;
				password = potentialPassword;
			}
			else if (qFuzzyCompare(newSimilarity, bestFit.similarity(serverSummary))
				&& candidate.time() > bestFit.time())
			{
				bestFit = candidate;
				password = potentialPassword;
			}
		}
	}
	return password;
}

