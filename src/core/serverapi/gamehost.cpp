//------------------------------------------------------------------------------
// gamehost.cpp
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
#include "gamehost.h"

#include "plugins/engineplugin.h"
#include "serverapi/message.h"
#include "serverapi/server.h"
#include "serverapi/binaries.h"
#include "serverapi/gamerunnerstructs.h"
#include "configuration/doomseekerconfig.h"
#include "gui/standardserverconsole.h"
#include "apprunner.h"
#include "log.h"
#include "main.h"
#include "pathfinder.h"
#include "strings.h"
#include <QDateTime>
#include <QStringList>

class GameHost::PrivData
{
	public:
		QString argIwadLoading;
		QString argPort;
		QString argPwadLoading;
		QString argDemoPlayback;
		QString argDemoRecord;
		QString argServerLaunch;

		CommandLineInfo* currentCmdLine;
		const HostInfo* currentHostInfo;
		const Server* server;
};

GameHost::GameHost(const Server* server)
{
	d = new PrivData();
	d->argIwadLoading = "-iwad";
	d->argPort = "-port";
	d->argPwadLoading = "-file";
	d->argDemoPlayback = "-playdemo";
	d->argDemoRecord = "-record";
	d->server = server;
	d->currentCmdLine = NULL;
	d->currentHostInfo = NULL;
}

GameHost::~GameHost()
{
	delete d;
}

const QString& GameHost::argForIwadLoading() const
{
	return d->argIwadLoading;
}

const QString& GameHost::argForPort() const
{
	return d->argPort;
}

const QString& GameHost::argForPwadLoading() const
{
	return d->argPwadLoading;
}

const QString& GameHost::argForDemoPlayback() const
{
	return d->argDemoPlayback;
}

const QString& GameHost::argForDemoRecord() const
{
	return d->argDemoRecord;
}

const QString& GameHost::argForServerLaunch() const
{
	return d->argServerLaunch;
}

#if 0
bool GameHost::connectParameters(QStringList &args, PathFinder &pf, bool &iwadFound, const QString &connectPassword, const QString &wadTargetDirectory)
{
	QString address = QString("%1:%2").arg(d->server->address().toString()).arg(d->server->port());

	// Connect
	args << argForConnect() << address;
	if(d->server->isLocked())
	{
		args << connectPassword;
	}

	// Iwad
	QString iwad = pf.findFile(d->server->iwad().toLower());
	args << argForIwadLoading() << iwad;
	iwadFound = !iwad.isEmpty();

	// Custom parameters
	IniSection config = gConfig.iniSectionForPlugin(d->server->plugin());
	QString customParameters = config["CustomParameters"];
	args << customParameters.split(" ", QString::SkipEmptyParts);

	// Password
	if (d->server->isLocked())
	{
		if (argForConnectPassword().isNull())
		{
			// TODO: Log a warning message here.
		}
		else
		{
			args << argForConnectPassword() << connectPassword;
		}
	}
	return true;
}
#endif

Message GameHost::createHostCommandLine(const HostInfo& hostInfo, CommandLineInfo& cmdLine, HostMode mode)
{
	Message message;

	d->currentCmdLine = &cmdLine;
	d->currentHostInfo = &hostInfo;

	cmdLine.args.clear();

	message = hostAppendIwad();
	if (!message.isIgnore())
	{
		return message;
	}

	message = hostAppendPwads();
	if (!message.isIgnore())
	{
		return message;
	}

	// Port
	if(mode == GameHost::HOST)
		cmdLine.args << argForPort() << QString::number(d->server->port());

	// CVars
	const QList<GameCVar>& cvars = hostInfo.cvars;
	foreach(const GameCVar c, cvars)
	{
		cmdLine.args << QString("+" + c.consoleCommand) << c.value();
	}

	message = hostGetBinary(mode != GameHost::HOST);
	if (!message.isIgnore())
	{
		return message;
	}

	message = hostGetWorkingDirectory(mode != GameHost::HOST);
	if (!message.isIgnore())
	{
		return message;
	}

	// Add the server launch parameter only if we don't want offline game
	if (mode == GameHost::HOST)
	{
		cmdLine.args << argForServerLaunch();
	}
	// Demo play back
	else if (mode == GameHost::DEMO)
	{
		cmdLine.args << argForDemoPlayback();
		cmdLine.args << hostInfo.demoPath;
	}

	hostDMFlags(cmdLine.args, hostInfo.dmFlags);
	hostProperties(cmdLine.args);
	cmdLine.args.append(hostInfo.customParameters);

	return message;
}

Message GameHost::host(const HostInfo& hostInfo, HostMode mode)
{
	CommandLineInfo cmdLine;

	Message message = createHostCommandLine(hostInfo, cmdLine, mode);
	if (!message.isIgnore())
	{
		return message;
	}

#ifdef Q_OS_WIN32
	const bool WRAP_IN_SSS_CONSOLE = false;
#else
	const bool WRAP_IN_SSS_CONSOLE = mode == HOST;
#endif

	return runExecutable(cmdLine, WRAP_IN_SSS_CONSOLE);
}

Message GameHost::hostAppendIwad()
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - appending IWAD");
	const QString& iwadPath = d->currentHostInfo->iwadPath;

	Message message;

	if (iwadPath.isEmpty())
	{
		message = Message::customError(tr("Iwad is not set"));
		return message;
	}

	QFileInfo fi(iwadPath);

	if (!fi.isFile())
	{
		QString error = tr("Iwad Path error:\n\"%1\" doesn't exist or is a directory!").arg(iwadPath);
		message = Message::customError(error);
		return message;
	}

	d->currentCmdLine->args << argForIwadLoading() << iwadPath;
	return message;
}

Message GameHost::hostAppendPwads()
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - appending PWADs");
	const QStringList& pwadsPaths = d->currentHostInfo->pwadsPaths;

	Message message;

	if (!pwadsPaths.isEmpty())
	{
		QStringList& args = d->currentCmdLine->args;
		foreach(const QString pwad, pwadsPaths)
		{
			args << argForPwadLoading();

			QFileInfo fi(pwad);
			if (!fi.isFile())
			{
				QString error = tr("Pwad path error:\n\"%1\" doesn't exist or is a directory!").arg(pwad);
				message = Message::customError(error);
				return message;
			}
			args << pwad;
		}
	}

	return message;
}

Message GameHost::hostGetBinary(bool bOfflinePlay)
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - getting executable");
	QString executablePath = d->currentHostInfo->executablePath;

	if (executablePath.isEmpty())
	{
		Binaries* binaries = d->server->binaries();

		Message message;

		// Select binary depending on bOfflinePlay flag:
		if (bOfflinePlay)
		{
			executablePath = binaries->offlineGameBinary(message);
		}
		else
		{
			executablePath = binaries->serverBinary(message);
		}

		delete binaries;

		if (executablePath.isEmpty())
		{
			return message;
		}
	}

	QFileInfo fi(executablePath);

	if (!fi.isFile() && !fi.isBundle())
	{
		Message message;
		QString error = tr("%1\n doesn't exist or is not a file.").arg(fi.filePath());
		message = Message::customError(error);
		return message;
	}

	d->currentCmdLine->executable = executablePath;
	return Message();
}

Message GameHost::hostGetWorkingDirectory(bool bOfflinePlay)
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - getting working directory");
	QString error;
	QString serverWorkingDirPath;

	Message message;
	
	// First, we should try to extract the working dir from plugin.
	// [Zalewa]:
	// A plugin may insist on doing that for a reason that is currently
	// unknown to me. Let's try to predict every possible situation.
	QString workingDirFromPlugin;
	Binaries* binaries = d->server->binaries();
	if (bOfflinePlay)
	{
		workingDirFromPlugin = binaries->offlineGameWorkingDirectory(message);
	}
	else
	{
		workingDirFromPlugin = binaries->serverWorkingDirectory(message);
	}
	
	// Check if all went well on the plugin side.
	if (!message.isIgnore())
	{
		// Something's gone wrong. Report the error.
		return message;
	}
	
	if (workingDirFromPlugin.isEmpty())
	{
		// Assume that working directory is the same as executable's directory.
		// Path to executable should be known at this point.
		QFileInfo fileInfo(d->currentCmdLine->executable);

		serverWorkingDirPath = fileInfo.absolutePath();
	}
	else
	{
		// Plugin returned the directory. Use that.
		serverWorkingDirPath = workingDirFromPlugin;
	}
	
	QDir serverWorkingDir(serverWorkingDirPath);

	if (serverWorkingDirPath.isEmpty())
	{
		QString error = tr("Path to working directory is empty.\nMake sure the configuration for the executable file is set properly.");
		message = Message::customError(error);
		return message;
	}
	else if (!serverWorkingDir.exists())
	{
		QString error = tr("%1\n doesn't exist or is not a directory.").arg(serverWorkingDirPath);
		message = Message::customError(error);
		return message;
	}

	d->currentCmdLine->applicationDir = serverWorkingDir;
	return message;
}

Message GameHost::runExecutable(const CommandLineInfo& cli, bool bWrapInStandardServerConsole)
{
	if (!bWrapInStandardServerConsole)
	{
		return AppRunner::runExecutable(cli);
	}
	else
	{
		gLog << tr("Starting (working dir %1): %2").arg(cli.applicationDir.absolutePath()).arg(cli.executable.absoluteFilePath());
		QStringList args = cli.args;
		// Is this needed for something? Zandronum needs the quotes for console
		// variables.
		//AppRunner::cleanArguments(args);
		new StandardServerConsole(d->server, cli.executable.absoluteFilePath(), args);
	}

	return Message();
}

void GameHost::setArgForIwadLoading(const QString& arg)
{
	d->argIwadLoading = arg;
}

void GameHost::setArgForPort(const QString& arg)
{
	d->argPort = arg;
}

void GameHost::setArgForPwadLoading(const QString& arg)
{
	d->argPwadLoading = arg;
}

void GameHost::setArgForDemoPlayback(const QString& arg)
{
	d->argDemoPlayback = arg;
}

void GameHost::setArgForDemoRecord(const QString& arg)
{
	d->argDemoRecord = arg;
}

void GameHost::setArgForServerLaunch(const QString& arg)
{
	d->argServerLaunch = arg;
}

