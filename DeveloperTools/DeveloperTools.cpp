#include "stdafx.h"
#include "DeveloperTools.h"

using namespace Olipro;

DeveloperTools DeveloperTools::self;

DeveloperTools::DeveloperTools() {
	CloseHandle(CreateThread(nullptr, 0, [](LPVOID) {
		self.console.Run();
		return 0ul;
	}, nullptr, 0, nullptr));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
