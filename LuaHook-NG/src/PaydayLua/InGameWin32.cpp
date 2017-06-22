#include "stdafx.h"
#include "PaydayLua/InGameWin32.h"
#include "FunctionHooker.h"
#include "ProcessSearcher.h"
#include "TempDetour.h"
#include "Utility.h"

using namespace Olipro;

InGameHandling* InGameHandling::self = nullptr;
decltype(InGameHandling::tGameTick) InGameHandling::tGameTick;
decltype(InGameHandling::tNewState) InGameHandling::tNewState;
decltype(InGameHandling::tCloseState) InGameHandling::tCloseState;
decltype(InGameHandling::tlua_setfield) InGameHandling::tlua_setfield;

InGameHandling::InGameHandling(	decltype(ProcessGameTick) onTick,
								decltype(ProcessNewState) onNew,
								decltype(ProcessCloseState) onClose) :
	ProcessGameTick(onTick), ProcessNewState(onNew),
	ProcessCloseState(onClose), functions(LoadSignatures())
{
	ProcessSearcher s;
	FunctionHooker hook;
	auto doGameTick = s.FindFunction<decltype(*tGameTick)&>(
		"\x56\xFF\x74\x24\x0C\x8B\xF1\xBA\xEE\xD8");
	hook.Attach(&doGameTick, &HookDoGameTick, &tGameTick);
	auto newState = &functions.luaL_newstate;
	hook.Attach(&newState, &HookNewState, &tNewState);
	auto closeState = &functions.lua_close;
	hook.Attach(&closeState, &HookLuaClose, &tCloseState);
}

InGameHandling& InGameHandling::GetInstance(decltype(ProcessGameTick) onTick,
	decltype(ProcessNewState) onNew, decltype(ProcessCloseState) onClose)
{
	static InGameHandling ret{ onTick, onNew, onClose };
	self = &ret;
	return ret;
}

InGameHandling& InGameHandling::GetInstance()
{
	if (!self)
		throw std::logic_error("Attempt to GetInstance before initialisation");
	return *self;
}

void InGameHandling::OnGameTick(lua_State* L, const char* op)
{
	GetInstance().ProcessGameTick(L, op);
}

void InGameHandling::HookNewState(lua_State** L, int edx, int a, int b, int c)
{
	auto self = GetInstance();
	{
		auto setFieldDetour = reinterpret_cast<FunctionHooker::UnsafePtr*>(
			&HookLuaSetField);
		auto setField = &self.functions.lua_setfield;
		TempDetour d{ &setField, setFieldDetour, &tlua_setfield };
		tNewState(L, edx, a, b, c);
	}
	self.ProcessNewState(*L);
}

void InGameHandling::OnLuaSetField(lua_State* L, int tbl, const char* k)
{
	const std::string name = k;
	if (tbl == LUA_GLOBALSINDEX && (name == "xpcall" || name == "pcall"))
		Lua::lua_settop(L, -2);
	else
		SafeCall(tlua_setfield, L, tbl, k);
}

void InGameHandling::HookLuaClose(lua_State* L)
{
	auto self = GetInstance();
	self.ProcessCloseState(L);
	self.tCloseState(L);
}

InGameHandling::InGameFunctionSignatures InGameHandling::LoadSignatures()
{
	using IGFS = InGameFunctionSignatures;
	ProcessSearcher s;
	return {
		s.FindFunction<decltype(IGFS::lua_checkstack)>(
			"\x56\x8B\xF1\x8B\x4E\x08\x8B\xC1\x2B\x46\x0C\xC1\xF8\x03\x03\xC2")
		, s.FindFunction<decltype(IGFS::lua_close)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x51\x56\x8B\x71\x10\x8B\x76\x70\x8B\xCE")
		, s.FindFunction<decltype(IGFS::lua_createtable)>(
			"\x53\x56\x57\x8B\xF9\x8B\xDA\x8B\x4F\x10\x8B\x41\x48\x85\xC0\x74"
			"\x0B\x48\x89\x41\x48\x8B\xCF\xE8\x84\xB2")
		, s.FindFunction<decltype(IGFS::lua_getfield)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x0C\x53\x56\x57\x8B\xF9\xE8\x5D")
		, s.FindFunction<decltype(IGFS::lua_getinfo)>(
			"\x53\x55\x56\x8B\xDA\x8B\x54\x24\x10\x57\x8B\xF1\x33\xFF\x33\xC9")
		, s.FindFunction<decltype(IGFS::lua_gettable)>(
			"\x56\x8B\xF1\xE8\x88\xF8\xFF\xFF\x8B\x56\x08\x83\xC2\xF8\x52\x52")
		, s.FindFunction<decltype(IGFS::lua_newuserdata)>(
			"\x51\x56\x8B\xF1\x57\x8B\x4E\x10\x8B\xFA\x8B\x41\x48\x85\xC0\x74")
		, s.FindFunction<decltype(IGFS::lua_objlen)>(
			"\x51\x56\x57\x8B\xF9\xE8\x36\xFB\xFF\xFF\x8B\xF0\x8B\x56\x04\x83")
		, s.FindFunction<decltype(IGFS::lua_pcall)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\xF2\x8B\x55\x0C\x57")
		, s.FindFunction<decltype(IGFS::lua_pushcclosure)>(
			"\x83\xEC\x08\x53\x55\x56\x8B\xF1\x57\x8B\x4E\x10\x89\x54\x24\x14\x8B\x41\x48")
		, s.FindFunction<decltype(IGFS::lua_pushlstring)>(
			"\x53\x56\x57\x8B\xF9\x8B\xDA\x8B\x4F\x10\x8B\x41\x48\x85\xC0\x74")
		, s.FindFunction<decltype(IGFS::lua_rawgeti)>(
			"\x56\x8B\xF1\xE8\xD8\xF7\xFF\xFF\x8B\x54\x24\x08\x8B\x08\xE8")
		, s.FindFunction<decltype(IGFS::lua_rawset)>(
			"\x51\x53\x55\x56\x57\x8B\xF1\xE8\x34\xF6\xFF\xFF\x8B\x5E\x08\x8B")
		, s.FindFunction<decltype(IGFS::lua_rawseti)>(
			"\x53\x56\x57\x8B\xF9\xE8\x86\xF5\xFF\xFF\xFF\x74\x24\x10\x8B\x77")
		, s.FindFunction<decltype(IGFS::lua_resume)>(
			"\x51\x53\x56\x8B\xF1\x8A\x46\x06\x3C\x01\x74\x28\x84\xC0\x74\x0E")
		, s.FindFunction<decltype(IGFS::lua_setfield)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x0C\x53\x56\x57\x8B\xF9\xE8\x8D")
		, s.FindFunction<decltype(IGFS::lua_setmetatable)>(
			"\x56\x57\x8B\xF1\xE8\x27\xF5\xFF\xFF\x8B\x4E\x08\x8B\xD0\x83\x79")
		, s.FindFunction<decltype(IGFS::lua_settop)>(
			"\x85\xD2\x78\x2E\x8B\x41\x0C\xC1\xE2\x03\x03\xC2\x39\x41\x08\x73")
		, s.FindFunction<decltype(IGFS::lua_tolstring)>(
			"\x56\x57\x8B\xFA\x8B\xF1\xE8\xB5\xFB\xFF\xFF\x8B\xC8\x83\x79\x04")
		, s.FindFunction<decltype(IGFS::luaC_fullgc)>(
			"\x56\x57\x8B\xF9\x8B\x77\x10\x80\x7E\x15\x01")
		, s.FindFunction<decltype(IGFS::luaC_step)>(
			"\x51\x53\x55\x56\x8B\xE9\x57\x8B\x7D\x10\x8B\x47\x5C\x8D\x1C\x80")
		, s.FindFunction<decltype(IGFS::luaD_call)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x56\x8B\xF1\xB9\xC8\x00\x00\x00\x66\xFF")
		, s.FindFunction<decltype(IGFS::luaD_protectedparser)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x14\x8B\x45\x08\x53\x56\x8B\xF1")
		, s.FindFunction<decltype(IGFS::luaE_newthread)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x6A\x64\x8B\xF1\x6A\x00")
		, s.FindFunction<decltype(IGFS::luaG_errormsg)>(
			"\x56\x8B\xF1\x57\x8B\x46\x60\x85\xC0\x74\x66\x8B\x7E\x20\x03\xF8")
		, s.FindFunction<decltype(IGFS::luaH_getnum)>(
			"\x51\x57\x8B\xF9\x8D\x42\xFF\x3B\x47\x1C\x73\x0C\x8B\x47\x0C\x5F")
		, s.FindFunction<decltype(IGFS::luaL_error)>(
			"\x56\x8B\x74\x24\x08\xBA\x01\x00\x00\x00\x8B\xCE\xE8\x4F\xFF\xFF")
		, s.FindFunction<decltype(IGFS::luaL_findtable)>(
			"\x83\xEC\x0C\x53\x55\x56\x57\x8B\xF9\xE8\xC2\xE4\xFF\xFF\x8B\x77")
		, s.FindFunction<decltype(IGFS::luaL_loadbuffer)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x20\x8B\x45\x08\x89\x44\x24\x04")
		, s.FindFunction<decltype(IGFS::luaL_loadfile)>(
			"\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x20\x02\x00\x00\x53\x55\x56\x57")
		, s.FindFunction<decltype(IGFS::luaL_newstate)>(
			"\x51\x8B\x44\x24\x10\x53\x56\x57\x8B\xF9\x85\xC0\x75\x07\xB9")
		, s.FindFunction<decltype(IGFS::luaO_pushvfstring)>(
			"\x83\xEC\x38\x53\x55\x56\x8B\xDA")
		, s.FindFunction<decltype(IGFS::luaV_settable)>(
			"\x83\xEC\x08\x53\x55\x56\x8B\xEA\x57\x89\x6C\x24\x10\x8B\xD9\xC7")
	};
}
