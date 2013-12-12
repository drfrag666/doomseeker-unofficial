//------------------------------------------------------------------------------
// gamehost.h
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
#ifndef idABA35264_C99B_49FA_BEDEDB4F5978C7EC
#define idABA35264_C99B_49FA_BEDEDB4F5978C7EC

#include "global.h"
#include <QObject>
#include <QStringList>

class CommandLineInfo;
class DMFlags;
struct HostInfo;
class Message;
class Server;

class MAIN_EXPORT GameHost : public QObject
{
	Q_OBJECT

	public:
		enum HostMode
		{
			HOST,
			OFFLINE,
			DEMO
		};

		GameHost(const Server* server);
		virtual ~GameHost();

		/**
		 * @param [out] cli - after successful call this will contain
		 *     required command line information.
		 * @param [out] error - if return == false, error text will be put here
		 * @param bOfflinePlay - if not HOST a command line for single player game
		 *     will be launched
		 *
		 * @return MessageResult::isError == false if command line was
		 *         successfully created.
		 */
		Message createHostCommandLine(const HostInfo& hostInfo, CommandLineInfo& cmdLine, HostMode mode);

		/**
		 * @see createHostCommandLine()
		 */
		Message host(const HostInfo& hostInfo, HostMode mode);

		/**
		 * @brief Executes predefined command line.
		 *
		 * @param cli - command line that will be executed
		 * @param bWrapWithStandardServerConsole - if true Doomseeker will
		 *     attempt to wrap the input/output of the program with it's own
		 *     console
		 */
		Message runExecutable(const CommandLineInfo& cli, bool bWrapWithStandardServerConsole);

	protected:
		/**
		 * @brief Command line parameter that is used to set IWAD.
		 *
		 * Default: "-iwad".
		 */
		const QString& argForIwadLoading() const;

		/**
		 * @brief Command line parameter that is used to set internet port for
		 * the game.
		 *
		 * Default: "-port".
		 */
		const QString& argForPort() const;

		/**
		 * @brief Command line parameter that is used to load a PWAD.
		 *
		 * Default: "-file".
		 */
		const QString& argForPwadLoading() const;

		/**
		 * @brief Command line parameter for playing back a demo.
		 *
		 * Default: "-playdemo".
		 */
		const QString& argForDemoPlayback() const;
		/**
		 * @brief Command line parameter for recording a demo.
		 *
		 * Default: "-record";
		 */
		const QString& argForDemoRecord() const;

		/**
		 * @brief Command line parameter used to launch a server.
		 *
		 * No default.
		 */
		const QString& argForServerLaunch() const;

		virtual Message hostAppendIwad();
		virtual Message hostAppendPwads();
		virtual Message hostGetBinary(bool bOfflinePlay);
		virtual Message hostGetWorkingDirectory(bool bOfflinePlay);

		/**
		 * @brief Creates engine specific command line parameters out of passed
		 * dmFlags list.
		 *
		 * Default behavior does nothing.
		 */
		virtual void hostDMFlags(QStringList& args, const DMFlags& dmFlags) const {};

		/**
		 * @brief Creates engine specific command line parameters out of
		 * Server class fields.
		 *
		 * Please note that port, and some other stuff, is already set by
		 * createHostCommandLine().
		 * @see createHostCommandLine() - cvars parameter.
		 */
		virtual void hostProperties(QStringList& args) const {};

		void setArgForIwadLoading(const QString& arg);
		void setArgForPort(const QString& arg);
		void setArgForPwadLoading(const QString& arg);
		void setArgForDemoPlayback(const QString& arg);
		void setArgForDemoRecord(const QString& arg);
		void setArgForServerLaunch(const QString& arg);

	private:
		class PrivData;
		PrivData* d;

		Q_DISABLE_COPY(GameHost);
};

#endif
