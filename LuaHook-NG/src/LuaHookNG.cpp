#include "stdafx.h"
#include "LuaHookNG.h"
#include "PaydayLua.h"

using namespace Olipro;

LuaHookNG::LuaHookNG(const std::wstring& game) : 
	luaInstance(GetLuaImplementation(game))
{
	CloseHandle(CreateThread(nullptr, 0, &DeferredInitialize,
								this, 0, nullptr));
}

DWORD LuaHookNG::DeferredInitialize(LPVOID inst)
{
	static_cast<LuaHookNG*>(inst)->console.Run();
	return 0;
}

LuaInterface& LuaHookNG::GetLuaImplementation(const std::wstring& game)
{
	if (game.rfind(L"payday2_win32_release.exe") != game.npos)
		return PaydayLua::Get();
	else if (game.rfind(L"payday_win32_release.exe") != game.npos)
		return PaydayLua::Get();
	throw "not found";
}

BOOL APIENTRY DllMain(	HMODULE hModule, DWORD ul_reason_for_call,
						LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		{
			std::array<TCHAR, MAX_PATH> exeName;
			GetModuleFileName(	GetModuleHandle(nullptr), exeName.data(),
								MAX_PATH);
			static LuaHookNG inst{ std::wstring{ exeName.data() } };
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
