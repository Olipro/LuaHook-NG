#include "ConsoleManager.h"

#include <array>
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fcntl.h>
#include <io.h>

using namespace LuaHookNG;
using namespace std::chrono_literals;

ConsoleManager::ConsoleManager() : link{*this} {
	if (!AllocConsole())
		throw "failed to Allocate console";

	auto closer = [](HANDLE h) {
		CloseHandle(h);
	};
	std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(closer)>
	  hConsoleHandle{CreateFile(L"CONOUT$",
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_WRITE,
								nullptr,
								OPEN_EXISTING,
								0,
								nullptr),
					 closer};

	realStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	realStdErr = GetStdHandle(STD_ERROR_HANDLE);
	realStdIn  = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleTitle(L"Debug Console");
	SetConsoleMode(hConsoleHandle.get(), ENABLE_EXTENDED_FLAGS);
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCtrlHandler(nullptr, TRUE);

	hConsoleHandle.reset(CreateFile(L"CONIN$",
									GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ,
									nullptr,
									OPEN_EXISTING,
									0,
									nullptr));

	SetConsoleMode(hConsoleHandle.get(),
				   ENABLE_LINE_INPUT | ENABLE_EXTENDED_FLAGS |
					 ENABLE_INSERT_MODE | ENABLE_PROCESSED_INPUT |
					 ENABLE_ECHO_INPUT);

	SetStdHandle(STD_OUTPUT_HANDLE, stdOut.write());
	SetStdHandle(STD_ERROR_HANDLE, stdErr.write());
	SetStdHandle(STD_INPUT_HANDLE, stdIn.read());

	auto newStdOut = _fdopen(
	  _open_osfhandle(reinterpret_cast<intptr_t>(stdOut.write()), _O_TEXT),
	  "w");
	auto newStdErr = _fdopen(
	  _open_osfhandle(reinterpret_cast<intptr_t>(stdErr.write()), _O_TEXT),
	  "w");
	auto newStdIn = _fdopen(
	  _open_osfhandle(reinterpret_cast<intptr_t>(stdIn.read()), _O_TEXT),
	  "r");

	usrCerr	 = std::ofstream(newStdOut);
	usrCout	 = std::ofstream(newStdErr);
	usrCin	 = std::ifstream(newStdIn);
	usrWcout = std::wofstream(newStdOut);
	usrWcerr = std::wofstream(newStdErr);
	usrWcin	 = std::wifstream(newStdIn);
	std::cout.rdbuf(usrCout.rdbuf());
	std::cerr.rdbuf(usrCerr.rdbuf());
	std::cin.rdbuf(usrCin.rdbuf());
	std::wcout.rdbuf(usrWcout.rdbuf());
	std::wcerr.rdbuf(usrWcerr.rdbuf());
	std::wcin.rdbuf(usrWcin.rdbuf());
	std::ios::sync_with_stdio();
	std::cout.setf(std::ios::unitbuf);
	std::wcout.setf(std::ios::unitbuf);
}

void ConsoleManager::Run() {
	outThread	 = AutoJoinThread{std::thread(&ConsoleManager::ProcessPipe,
											  this,
											  stdOut.read(),
											  realStdOut,
											  &logFile,
											  true)};
	errThread	 = AutoJoinThread{std::thread(&ConsoleManager::ProcessPipe,
											  this,
											  stdErr.read(),
											  realStdErr,
											  &logFile,
											  false)};
	inThread	 = AutoJoinThread{std::thread(&ConsoleManager::ProcessPipe,
										  this,
										  realStdIn,
										  stdIn.write(),
										  &logFile,
										  false)};
	consoleInput = {&ConsoleManager::ConsoleInputReader, this};
}

void ConsoleManager::WriteToPrompt(const std::string& prompt) {
	WriteFile(realStdOut, prompt.c_str(), prompt.length(), nullptr, nullptr);
}

std::string ConsoleManager::ConsoleInputReader() {
	std::string str;
	if (!std::cin.eof()) {
		skip = true;
		std::cout << "\nLua> ";
		std::getline(std::cin, str);
	}
	return str;
}

void ConsoleManager::OnTick(lua_State* L) {
	if (consoleInput.wait_for(0s) == std::future_status::ready) {
		auto result		= consoleInput.get();
		auto str		= result.get();
		const char *err = nullptr, *out = nullptr;
		if (luaL_loadbuffer(L, str.c_str(), str.length(), "Console"))
			err = lua_tolstring(L, -1, nullptr);
		else if (lua_pcall(L, 0, LUA_MULTRET, 0))
			err = lua_tolstring(L, -1, nullptr);
		else if (lua_gettop(L))
			out = lua_tolstring(L, -1, nullptr);
		if (err)
			std::cerr << err << "\n";
		if (out)
			std::cout << out << "\n";
	}
}

void ConsoleManager::ProcessPipe(HANDLE rPipe,
								 HANDLE wPipe,
								 std::ofstream* const file,
								 bool doRefresh) {
	constexpr size_t bufSize = 4096, timeout = 200;
	DWORD numBytes, remBytes;
	std::array<char, bufSize> buf;
	bool shouldSkip = true;
	while (ReadFile(rPipe, buf.data(), buf.size(), &numBytes, nullptr) &&
		   numBytes) {
		if (file != nullptr)
			file->write(buf.data(), numBytes), file->flush();
		WriteFile(wPipe, buf.data(), numBytes, &remBytes, nullptr);
		if (doRefresh && buf.back() == '\n' &&
			!skip.compare_exchange_strong(shouldSkip, false)) {
			shouldSkip = true;
			for (auto i = 0;
				 PeekNamedPipe(rPipe, nullptr, 0, nullptr, &remBytes, nullptr),
					  !remBytes && i < timeout;
				 i++)
				std::this_thread::sleep_for(1ms);
			if (!remBytes)
				WriteToPrompt("\nLua> ");
		}
	}
}
