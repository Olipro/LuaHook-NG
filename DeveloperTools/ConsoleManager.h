#pragma once
#include "AutoJoinThread.h"
#include "CyclicFuture.h"
#include "PipePair.h"
#include <LuaGame.h>

namespace Olipro {
	class ConsoleManager {
		PipePair stdOut, stdErr, stdIn;
		std::ifstream usrCin;
		std::ofstream usrCout, usrCerr;
		std::wifstream usrWcin;
		std::wofstream usrWcout, usrWcerr;
		std::ofstream logFile{ "DevLog.txt", std::ofstream::out };
		HANDLE realStdOut, realStdErr, realStdIn;
		std::atomic_bool skip;
		AutoJoinThread inThread, outThread, errThread;
		CyclicFuture<std::string> consoleInput;
		const LuaGame game;

		void ProcessPipe(HANDLE, HANDLE, std::ofstream* const, bool);
		void WriteToPrompt(const std::string&);
		std::string ConsoleInputReader();
		void LuaCodeRunner(lua_State*, LuaInterface&);

		
	public:
		ConsoleManager();
		void Run();
	};
}
