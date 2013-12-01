//------------------------------------------------------------------------------
// binaries.h
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
#ifndef __BINARIES_H_
#define __BINARIES_H_

#include "global.h"
#include <QHash>
#include <QObject>
#include <QString>

class EnginePlugin;
class Message;

class MAIN_EXPORT Binaries : public QObject
{
	Q_OBJECT

	public:
		Binaries(const EnginePlugin *plugin);

		/**
		 * @brief Returns the path to the client binary.
		 *
		 * @param [out] message - information message, if any.
		 * @return Empty if error is not empty.
		 */
		virtual QString clientBinary(Message& message) const { return obtainBinary(configKeyClientBinary(), Client, message); }

		/**
		 * Default behavior returns directory of clientBinary(), but
		 * you can override this to provide different working directory for
		 * Skulltag's testing binaries.
		 *
		 * @param [out] error - type of error
		 */
		virtual QString clientWorkingDirectory(Message& message) const;

		virtual QString configKeyClientBinary() const { return "BinaryPath"; }
		virtual QString configKeyOfflineBinary() const { return configKeyClientBinary(); }
		virtual QString configKeyServerBinary() const { return "ServerBinaryPath"; }

		/**
		 * @brief Returns the path to the binary for offline play.
		 *
		 * @param [out] message - information message, if any.
		 * @return Empty string if error is not empty.
		 */
		virtual QString offlineGameBinary(Message& message) const { return obtainBinary(configKeyOfflineBinary(), Offline, message); }

		/**
		 * @brief Working directory for offline play.
		 *
		 * Default behavior prompts Doomseeker to use a path to the directory
		 * where the executable selected for the offline game resides. 
		 * This executable is always selected directly in Create Server dialog.
		 *
		 * @note
		 * This method should be overriden ONLY IF it is ABSOLUTELY required
		 * to return a non-standard path to make the game run correctly.
		 *
		 * @return Path to the working directory for the server binary.
		 *         Default behavior returns an empty string.
		 */
		virtual QString offlineGameWorkingDirectory(Message& message) const;

		/**
		 * This is supposed to return the plugin this Binaries belongs to.
		 * New instances of EnginePlugin shouldn't be created here. Instead
		 * each plugin should keep a global instance of EnginePlugin (singleton?)
		 * and a pointer to this instance should be returned.
		 */
		const EnginePlugin* plugin() const { return enginePlugin; }

		/**
		 * @brief Returns the path to the server binary.
		 *
		 * @param [out] message - information message, if any.
		 * @return Empty if error is not empty.
		 */
		virtual QString serverBinary(Message& message) const;

		/**
		 * @brief Working directory for server.
		 *
		 * Default behavior prompts Doomseeker to use a path to the directory
		 * where the executable selected for the server launch resides. 
		 * This executable is always selected directly in Create Server dialog.
		 *
		 * @note
		 * This method should be overriden ONLY IF it is ABSOLUTELY required
		 * to return a non-standard path to make the game run correctly.
		 *
		 * @return Path to the working directory for the server binary.
		 *         Default behavior returns an empty string.
		 */
		virtual QString serverWorkingDirectory(Message& message) const;

	protected:
		enum BinaryType
		{
			Client,
			Offline,
			TServer
		};

		/**
		 * @brief A dictionary containing enum to names translations.
		 *
		 * Names should fit into "No <dictionary_word> executable specified
		 * [...]" sentence.
		 *
		 * Initialized when Binaries is first constructed.
		 */
		typedef QHash<BinaryType, QString> BinaryNamesDictionary;

		static BinaryNamesDictionary binaryNames;

		QString obtainBinary(const QString& configKey, BinaryType binaryType, Message& message) const;

		const EnginePlugin *enginePlugin;
};

#endif
