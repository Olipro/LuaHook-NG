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
	auto&& self = *static_cast<LuaHookNG*>(inst);
	self.console.Run();
	self.LoadClientLibraries();
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

void LuaHookNG::LoadClientLibraries()
{
	using namespace std::experimental::filesystem;
	for (auto&& i : directory_iterator{ "LuaHookNG" })
		if (is_regular_file(i.path()))
			LoadLibraryW(i.path().wstring().c_str());
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
			static Olipro::LuaHookNG inst{ std::wstring{ exeName.data() } };
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void LuaHookNG()
{

}
