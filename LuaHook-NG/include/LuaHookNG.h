#pragma once
#include "LuaInterface.h"

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID);

namespace Olipro {
	class LuaHookNG {
		LuaInterface& luaInstance;

		LuaInterface& GetLuaImplementation(const std::wstring&);
		void LoadClientLibraries();
		
		static DWORD WINAPI DeferredInitialize(LPVOID);

	public:
		LuaHookNG(const std::wstring&);
	};
}

extern "C" EXTERNAL void LuaHookNG();
