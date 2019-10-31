#include <array>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "detail/LuaAppSelector.h"
#include "detail/windows/DetoursCodeHook.h"
#include "detail/windows/WindowsLibraryLoader.h"

using namespace LuaHookNG;

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
			{
				std::array<TCHAR, MAX_PATH> exeName;
				auto len = GetModuleFileName(GetModuleHandle(nullptr),
											 exeName.data(),
											 MAX_PATH);
				static DetoursCodeHook codeHook;
				static WindowsLibraryLoader libLoader;
				static LuaAppSelector inst{{exeName.data(), len},
										   codeHook,
										   libLoader};
			}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
