//------------------------------------------------------------------------------
// gamerunner.cpp
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
#include "serverapi/gamerunner.h"
#include "serverapi/messages.h"
#include "serverapi/server.h"
#include "serverapi/binaries.h"
#include "gui/standardserverconsole.h"
#include "apprunner.h"
#include "log.h"
#include "main.h"
#include "pathfinder.h"
#include "strings.h"
#include <QStringList>

GameRunner::GameRunner(const Server* server, IniSection &config)
: currentCmdLine(NULL), currentHostInfo(NULL), server(server), config(config)
{
}

void GameRunner::connectParameters(QStringList &args, PathFinder &pf, bool &iwadFound, const QString &connectPassword)
{
	QString address = QString("%1:%2").arg(server->address().toString()).arg(server->port());

	// Connect
	args << argForConnect() << address;
	if(server->isLocked())
	{
		args << connectPassword;
	}

	// Iwad
	QString iwad = pf.findFile(server->iwadName().toLower());
	args << argForIwadLoading() << iwad;
	iwadFound = !iwad.isEmpty();

	// Custom parameters
	QString customParameters = *config["CustomParameters"];
	args << customParameters.split(" ", QString::SkipEmptyParts);

	// Password
	if (server->isLocked())
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
}

Message GameRunner::createHostCommandLine(const HostInfo& hostInfo, CommandLineInfo& cmdLine, bool bOfflinePlay)
{
	const QString RUN_RESULT_CAPTION = tr("createHostCommandLine");
	Message message;

	currentCmdLine = &cmdLine;
	currentHostInfo = &hostInfo;

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
	cmdLine.args << argForPort() << QString::number(server->port());

	// CVars
	const QList<GameCVar>& cvars = hostInfo.cvars;
	foreach(const GameCVar c, cvars)
	{
		cmdLine.args << QString("+" + c.consoleCommand) << c.value();
	}

	message = hostGetBinary(bOfflinePlay);
	if (!message.isIgnore())
	{
		return message;
	}

	message = hostGetWorkingDirectory(bOfflinePlay);
	if (!message.isIgnore())
	{
		return message;
	}

	// Add the server launch parameter only if we don't want offline game
	if (!bOfflinePlay)
	{
		cmdLine.args << argForServerLaunch();
	}

	hostDMFlags(cmdLine.args, hostInfo.dmFlags);
	hostProperties(cmdLine.args);
	cmdLine.args.append(hostInfo.customParameters);

	return message;
}

JoinError GameRunner::createJoinCommandLine(CommandLineInfo& cli, const QString &connectPassword)
{
	const QString PLUGIN_NAME = this->plugin()->name;
	JoinError joinError;

	// Init the JoinError type with critical error. We will change this upon
	// successful return or if wads are missing.
	joinError.type = JoinError::Critical;

	QDir& applicationDir = cli.applicationDir;
	QFileInfo& executablePath = cli.executable;
	QStringList& args = cli.args;

	//const QString errorCaption = tr("Doomseeker - error for plugin");
	args.clear();

	Binaries* binaries = server->binaries();

	Message message;
	QString clientBin = binaries->clientBinary(message);
	if (clientBin.isEmpty())
	{
		joinError.error = tr("Client binary cannot be obtained for game: %1").arg(PLUGIN_NAME);
		if (!message.isIgnore())
		{
			joinError.error += "\n" + message.content;
		}

		delete binaries;
		return joinError;
	}

	executablePath = clientBin;

	QString clientWorkingDirPath = binaries->clientWorkingDirectory(message);
	applicationDir = clientWorkingDirPath;

	delete binaries;

	if (clientWorkingDirPath.isEmpty())
	{
		joinError.error = tr("Path to working directory for \"%1\" is empty.\nMake sure the configuration for the main binary is set properly.").arg(PLUGIN_NAME);
		return joinError;
	}
	else if (!applicationDir.exists())
	{
		joinError.error = tr("%1\n cannot be used as working directory for game:\n%2\nExecutable: %3").arg(clientWorkingDirPath, PLUGIN_NAME, clientBin);
		return joinError;
	}

	PathFinder pf(Main::config);
	QStringList missingPwads;
	bool iwadFound = false;

	connectParameters(cli.args, pf, iwadFound, connectPassword);

	for (int i = 0; i < server->numWads(); ++i)
	{
		QString pwad = pf.findFile(server->wad(i).name);
		if (pwad.isEmpty() && !server->wad(i).optional)
		{
			missingPwads << server->wad(i).name;
		}
		else
		{
			cli.args << argForPwadLoading();
			cli.args << pwad;
		}
	}

	if (!iwadFound || !missingPwads.isEmpty())
	{
		if (!iwadFound)
		{
			joinError.missingIwad = server->iwadName();
		}
		joinError.missingWads = missingPwads;
		joinError.type = JoinError::MissingWads;
		return joinError;
	}

	joinError.type = JoinError::NoError;
	return joinError;
}

Message GameRunner::host(const HostInfo& hostInfo, bool bOfflinePlay)
{
	CommandLineInfo cmdLine;

	Message message = createHostCommandLine(hostInfo, cmdLine, bOfflinePlay);
	if (!message.isIgnore())
	{
		return message;
	}

#ifdef Q_OS_WIN32
	const bool WRAP_IN_SSS_CONSOLE = false;
#else
	const bool WRAP_IN_SSS_CONSOLE = !bOfflinePlay;
#endif

	return runExecutable(cmdLine, WRAP_IN_SSS_CONSOLE);
}

Message GameRunner::hostAppendIwad()
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - appending IWAD");
	const QString& iwadPath = currentHostInfo->iwadPath;

	Message message;

	if (iwadPath.isEmpty())
	{
		message.setCustomError(tr("Iwad is not set"));
		return message;
	}

	QFileInfo fi(iwadPath);

	if (!fi.isFile())
	{
		QString error = tr("Iwad Path error:\n\"%1\" doesn't exist or is a directory!").arg(iwadPath);
		message.setCustomError(error);
		return message;
	}

	currentCmdLine->args << argForIwadLoading() << iwadPath;
	return message;
}

Message GameRunner::hostAppendPwads()
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - appending PWADs");
	const QStringList& pwadsPaths = currentHostInfo->pwadsPaths;

	Message message;

	if (!pwadsPaths.isEmpty())
	{
		QStringList& args = currentCmdLine->args;
		foreach(const QString pwad, pwadsPaths)
		{
			args << argForPwadLoading();

			QFileInfo fi(pwad);
			if (!fi.isFile())
			{
				QString error = tr("Pwad path error:\n\"%1\" doesn't exist or is a directory!").arg(pwad);
				message.setCustomError(error);
				return message;
			}
			args << pwad;
		}
	}

	return message;
}

Message GameRunner::hostGetBinary(bool bOfflinePlay)
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - getting executable");
	QString executablePath = currentHostInfo->executablePath;

	if (executablePath.isEmpty())
	{
		Binaries* binaries = server->binaries();

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
		message.setCustomError(error);
		return message;
	}

	currentCmdLine->executable = executablePath;
	return Message();
}

Message GameRunner::hostGetWorkingDirectory(bool bOfflinePlay)
{
	const QString RESULT_CAPTION = tr("Doomseeker - host - getting working directory");
	QString error;
	QString serverWorkingDirPath;

	Message message;

	Binaries* binaries = server->binaries();
	// Select working dir based on bOfflinePlay flag:
	if (bOfflinePlay)
	{
		serverWorkingDirPath = binaries->offlineGameWorkingDirectory(message);
	}
	else
	{
		serverWorkingDirPath = binaries->serverWorkingDirectory(message);
	}

	delete binaries;
	QDir applicationDir = serverWorkingDirPath;

	if (serverWorkingDirPath.isEmpty())
	{
		QString error = tr("Path to working directory is empty.\nMake sure the configuration for the main binary is set properly.");
		message.setCustomError(error);
		return message;
	}
	else if (!applicationDir.exists())
	{
		QString error = tr("%1\n doesn't exist or is not a directory.").arg(serverWorkingDirPath);
		message.setCustomError(error);
		return message;
	}

	currentCmdLine->applicationDir = applicationDir;
	return message;
}

Message GameRunner::runExecutable(const CommandLineInfo& cli, bool bWrapInStandardServerConsole)
{
	if (!bWrapInStandardServerConsole)
	{
		return AppRunner::runExecutable(cli);
	}
	else
	{
		gLog << tr("Starting (working dir %1): %2 %3").arg(cli.applicationDir.canonicalPath()).arg(cli.executable.canonicalFilePath()).arg(cli.args.join(" "));
		QStringList args = cli.args;
		AppRunner::cleanArguments(args);
		new StandardServerConsole(server, cli.executable.canonicalFilePath(), args);
	}

	return Message();
}
