#pragma once

#include "UpdateInstaller.h"

/** Parses the command-line options to the updater binary. */
class UpdaterOptions
{
	public:
		UpdaterOptions();

		void parse(int argc, char** argv);

		UpdateInstaller::Mode mode;
		std::string installDir;
		std::string packageDir;
		std::string scriptPath;
		PLATFORM_PID waitPid;
		std::string logFile;
		/// Arguments passed to the program when installation is done.
		std::list<std::string> runAfterInstallCmdArgs;
		bool showVersion;
		bool forceElevated;

	private:
		std::list<std::string> parseArgs(const std::string& args);
		void parseOldFormatArgs(int argc, char** argv);
		static void parseOldFormatArg(const std::string& arg, std::string* key, std::string* value);
};

