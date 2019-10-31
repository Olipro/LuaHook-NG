#include "DeveloperTools.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace LuaHookNG;

DeveloperTools DeveloperTools::self;

static DWORD __stdcall threadWrapper(LPVOID l) {
	return reinterpret_cast<DWORD (*)(LPVOID)>(l)(nullptr);
}

DeveloperTools::DeveloperTools() {
	DWORD (*threadFunc)
	(LPVOID) = [](LPVOID) {
		self.console.Run();
		return 0ul;
	};
	CloseHandle(CreateThread(nullptr,
							 0,
							 &threadWrapper,
							 reinterpret_cast<void*>(threadFunc),
							 0,
							 nullptr));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
