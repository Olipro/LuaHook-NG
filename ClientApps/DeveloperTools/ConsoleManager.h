#pragma once
#include "AutoJoinThread.h"
#include "CyclicFuture.h"
#include "LuaHookNG.h"
#include "PipePair.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <fstream>
#include <string>

namespace LuaHookNG {
	class ConsoleManager : public IOnTickClient {
		PipePair stdOut, stdErr, stdIn;
		std::ifstream usrCin;
		std::ofstream usrCout, usrCerr;
		std::wifstream usrWcin;
		std::wofstream usrWcout, usrWcerr;
		std::ofstream logFile{"DevLog.txt", std::ofstream::out};
		HANDLE realStdOut, realStdErr, realStdIn;
		std::atomic_bool skip;
		AutoJoinThread inThread, outThread, errThread;
		CyclicFuture<std::string> consoleInput;
		LuaAppLink link;

		void ProcessPipe(HANDLE, HANDLE, std::ofstream* const, bool);
		void WriteToPrompt(const std::string&);
		std::string ConsoleInputReader();

		void OnTick(lua_State*) override;

	public:
		ConsoleManager();
		void Run();
	};
}  // namespace LuaHookNG
