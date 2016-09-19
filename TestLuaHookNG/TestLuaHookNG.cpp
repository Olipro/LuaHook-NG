// TestLuaHookNG.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "LuaGame.h"
#include <iostream>
#include <string>

using namespace Olipro;

class TestLuaHookNG {
	static TestLuaHookNG self;
	static const char* const luaPrint;
	LuaGame pd2;

	static void PrintString(lua_State* L, LuaInterface& lua,
		const char* const str) {
		lua.luaL_loadstring(L, luaPrint);
		lua.lua_pushstring(L, str);
		auto result = lua.lua_pcall(L, 1, 0, 0);
		if (result) {
			self.pd2 << std::to_string(result) << "\n";
		}
	}

	static void OnNewState(lua_State* L, LuaInterface& lua) {
		PrintString(L, lua, "OnNewState\n");
	}

	static void OnGameTick(lua_State* L, LuaInterface& lua) {
		PrintString(L, lua, "OnGameTick\n");
	}

	static void OnCloseState(lua_State* L, LuaInterface& lua) {
		PrintString(L, lua, "OnCloseState\n");
	}

	static void OnRequire(lua_State* L, LuaInterface& lua, const std::string s)
	{
		PrintString(L, lua, (std::string("Require: ") + s + "\n").c_str());
	}

	TestLuaHookNG() : pd2{ OnGameTick, OnNewState, OnCloseState, OnRequire }
	{
		pd2 << "constructed\n";
	}

	~TestLuaHookNG() {
		pd2 << "destructed\n";
	}
};

TestLuaHookNG TestLuaHookNG::self;
const char* const TestLuaHookNG::luaPrint = "io.stdout:write(...)";

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason,LPVOID lpReserved)
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

