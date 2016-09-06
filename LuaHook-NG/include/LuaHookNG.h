#pragma once
#include "ConsoleManager.h"
#include "LuaInterface.h"

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID);

namespace Olipro {
	class LuaHookNG {
		LuaInterface& luaInstance;
		ConsoleManager console;

		LuaInterface& GetLuaImplementation(const std::wstring&);
		LuaHookNG(const std::wstring&);
		
		static DWORD WINAPI DeferredInitialize(LPVOID);
		friend BOOL APIENTRY ::DllMain(HMODULE, DWORD, LPVOID);
	};
}
