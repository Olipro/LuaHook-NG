#include "CatchMain.h"
#define CATCH_CONFIG_RUNNER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <shellapi.h>

#include <catch.hpp>

static DWORD __stdcall lambdaWrapper(void* lambda) {
	return reinterpret_cast<DWORD (*)(void*)>(lambda)(nullptr);
}

void TestLuaHookNG::CatchMain() {
	static Catch::Session session;
	auto config				   = session.configData();
	config.showSuccessfulTests = true;
	config.useColour		   = Catch::UseColour::Yes;
	config.showDurations	   = Catch::ShowDurations::Always;
	session.useConfigData(config);

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
	AllocConsole();
	SetConsoleTitle(L"Debug Console");
	SetConsoleMode(hConsoleHandle.get(), ENABLE_EXTENDED_FLAGS);
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCtrlHandler(nullptr, TRUE);

	auto cmdLine   = GetCommandLine();
	auto argc	   = 0;
	auto localFree = [](wchar_t** s) {
		LocalFree(s);
	};
	std::unique_ptr<wchar_t*, decltype(localFree)> argv{
	  static_cast<wchar_t**>(CommandLineToArgvW(cmdLine, &argc)),
	  localFree};
	session.applyCommandLine(argc, argv.get());
	DWORD (*threadWorker)
	(void*) = [](void* arg) -> DWORD {
		return session.run();
	};
	auto t = CreateThread(nullptr,
						  1024 * 1024,
						  &lambdaWrapper,
						  reinterpret_cast<void*>(threadWorker),
						  0,
						  nullptr);
	if (t)
		CloseHandle(t);
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);
	std::ios::sync_with_stdio();
}
