#include "stdafx.h"
#include "FunctionHooker.h"
#include "LuaGameImpl.h"
#include "LuaHookNG.h"
#include "PaydayLua.h"
#include "ProcessSearcher.h"
#include "TempDetour.h"
#include "Utility.h"

using namespace Olipro;

PaydayLua::InGameFunctionSignatures PaydayLua::inGame;

PaydayLua& PaydayLua::Get()
{
	static PaydayLua self;
	return self;
}

PaydayLua::PaydayLua()
{
	ProcessSearcher sigSearcher;
	FunctionHooker hooker;

	auto doGameTick = inGame.tDoGameTick;
	sigSearcher.FindFunction(doGameTick,
		"\x56\xFF\x74\x24\x0C\x8B\xF1\xBA\xEE\xD8\xFF\xFF\x8B\x0E\xE8\x3D"s);
	sigSearcher.FindFunction(inGame.lua_close,
		"\x55\x8B\xEC\x83\xE4\xF8\x51\x56\x8B\x71\x10\x8B\x76\x70\x8B\xCE"s);
	sigSearcher.FindFunction(inGame.lua_createtable,
		"\x53\x56\x57\x8B\xF9\x8B\xDA\x8B\x4F\x10\x8B\x41\x48\x85\xC0\x74"
		"\x0B\x48\x89\x41\x48\x8B\xCF\xE8\x64\xB3");
	sigSearcher.FindFunction(inGame.lua_getfield,
		"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x0C\x53\x56\x57\x8B\xF9\xE8\x5D"s);
	sigSearcher.FindFunction(inGame.lua_gettable,
		"\x56\x8B\xF1\xE8\x88\xF8\xFF\xFF\x8B\x56\x08\x83\xC2\xF8\x52\x52"s);
	sigSearcher.FindFunction(inGame.lua_newuserdata,
		"\x51\x56\x8B\xF1\x57\x8B\x4E\x10\x8B\xFA\x8B\x41\x48\x85\xC0\x74");
	sigSearcher.FindFunction(inGame.lua_objlen,
		"\x51\x56\x57\x8B\xF9\xE8\x36\xFB\xFF\xFF\x8B\xF0\x8B\x56\x04\x83"s);
	sigSearcher.FindFunction(inGame.lua_pcall,
		"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\xF2\x8B\x55\x0C\x57"s);
	sigSearcher.FindFunction(inGame.lua_pushlstring,
		"\x53\x56\x57\x8B\xF9\x8B\xDA\x8B\x4F\x10\x8B\x41\x48\x85\xC0\x74"s);
	sigSearcher.FindFunction(inGame.lua_pushcclosure,
		"\x83\xEC\x08\x53\x55\x56\x8B\xF1\x57\x8B\x4E\x10\x89\x54\x24\x14");
	sigSearcher.FindFunction(inGame.lua_rawgeti,
		"\x56\x8B\xF1\xE8\xD8\xF7\xFF\xFF\x8B\x54\x24\x08\x8B\x08\xE8\x1D");
	sigSearcher.FindFunction(inGame.lua_rawset,
		"\x51\x53\x55\x56\x57\x8B\xF1\xE8\x34\xF6\xFF\xFF\x8B\x5E\x08\x8B"s);
	sigSearcher.FindFunction(inGame.lua_rawseti,
		"\x53\x56\x57\x8B\xF9\xE8\x86\xF5\xFF\xFF\xFF\x74\x24\x10\x8B\x77"s);
	sigSearcher.FindFunction(inGame.lua_setfield,
		"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x0C\x53\x56\x57\x8B\xF9\xE8\x8D"s);
	sigSearcher.FindFunction(inGame.lua_settop,
		"\x85\xD2\x78\x2E\x8B\x41\x0C\xC1\xE2\x03\x03\xC2\x39\x41\x08\x73"s);
	sigSearcher.FindFunction(inGame.lua_tolstring,
		"\x56\x57\x8B\xFA\x8B\xF1\xE8\xB5\xFB\xFF\xFF\x8B\xC8\x83\x79\x04"s);
	sigSearcher.FindFunction(inGame.luaC_fullgc,
		"\x56\x57\x8B\xF9\x8B\x77\x10\x80\x7E\x15\x01\x77\x26\x8D\x46\x1C"s);
	sigSearcher.FindFunction(inGame.luaC_step,
		"\x51\x53\x55\x56\x8B\xE9\x57\x8B\x7D\x10\x8B\x47\x5C\x8D\x1C\x80"s);
	sigSearcher.FindFunction(inGame.luaD_call,
		"\x55\x8B\xEC\x83\xE4\xF8\x56\x8B\xF1\xB9\xC8\x00\x00\x00\x66\xFF"s);
	sigSearcher.FindFunction(inGame.luaD_protectedparser,
		"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x14\x8B\x45\x08\x53\x56\x8B\xF1");
	sigSearcher.FindFunction(inGame.luaE_newthread,
		"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x6A\x64\x8B\xF1\x6A\x00"s);
	sigSearcher.FindFunction(inGame.luaG_errormsg,
		"\x56\x8B\xF1\x57\x8B\x46\x60\x85\xC0\x74\x66\x8B\x7E\x20\x03\xF8"s);
	sigSearcher.FindFunction(inGame.luaH_getnum,
		"\x51\x57\x8B\xF9\x8D\x42\xFF\x3B\x47\x1C\x73\x0C\x8B\x47\x0C\x5F"s);
	sigSearcher.FindFunction(inGame.luaL_loadbuffer,
		"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x20\x8B\x45\x08\x89\x44\x24\x04"s);
	sigSearcher.FindFunction(inGame.luaL_loadfile,
		"\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x20\x02\x00\x00\x53\x55\x56\x57"s);
	sigSearcher.FindFunction(inGame.luaL_newstate,
		"\x51\x8B\x44\x24\x10\x53\x56\x57\x8B\xF9\x85\xC0\x75\x07\xB9\x80"s);
	sigSearcher.FindFunction(inGame.luaL_ref,
		"\x83\xEC\x0C\x56\x8B\xF1\x57\x8B\x46\x08\x83\xC0\xF8\x3D\xB4\x2B"s);
	sigSearcher.FindFunction(inGame.luaL_unref,
		"\x56\x57\x8B\x7C\x24\x0C\x8B\xF1\x85\xFF\x78\x5B\xBA\xF0\xD8\xFF"s);
	sigSearcher.FindFunction(inGame.luaO_pushvfstring,
		"\x83\xEC\x38\x53\x55\x56\x8B\xDA\x57\xBD\x01\x00\x00\x00\xBA\xE5");
	sigSearcher.FindFunction(inGame.luaV_settable,
		"\x83\xEC\x08\x53\x55\x56\x8B\xEA\x57\x89\x6C\x24\x10\x8B\xD9\xC7"s);

	auto unsafeHookTick = reinterpret_cast<FunctionHooker::UnsafePtr*>(
		&HookDoGameTick);
	hooker.Attach(&doGameTick, unsafeHookTick, &inGame.tDoGameTick);
	hooker.Attach(&inGame.lua_close, HookLuaClose, &inGame.tLuaClose);
	hooker.Attach(	&inGame.luaL_newstate, HookLuaLNewState,
					&inGame.tLuaL_newstate);
}

__declspec(naked) void PaydayLua::HookDoGameTick()
{
	__asm {
		mov edx, [esp+8]
		push ecx
		mov ecx, [ecx]
		call ProcessGameTick
		pop ecx
		jmp inGame.tDoGameTick
	}
}

void PaydayLua::ProcessGameTick(lua_State* L, LPCSTR op)
{
	if (Str2Uint(op) == Str2Uint("update")) {
		SafeCall(inGame.lua_pushcclosure, L, [](auto L) {
			return Get().OnGameTick(L), 0;
		}, 0);
		Get().lua_pcall(L, 0, 0, 0);
	}
}

void PaydayLua::HookLuaSetField(const char* k)
{
	lua_State *L;
	int tbl;
	__asm mov L, ecx
	__asm mov tbl, edx
	std::string name = k;
	if (tbl == LUA_GLOBALSINDEX && (name == "xpcall" || name == "pcall"))
		Get().lua_settop(L, -2);
	else
		SafeCall(inGame.tLua_setfield, L, tbl, k);
}

void PaydayLua::HookLuaLNewState(lua_State** L, int edx, int a, int b, int c)
{
	{
		auto SetFieldDetour = reinterpret_cast<FunctionHooker::UnsafePtr*>(
			&HookLuaSetField);
		TempDetour detour{ &inGame.lua_setfield, SetFieldDetour,
							&inGame.tLua_setfield };
		inGame.luaL_newstate(L, edx, a, b, c);
	}
	SafeCall(inGame.lua_pushcclosure, *L, [](auto L) {
		Get().OnNewState(L);
		HookRequire(L);
		return 0;
	}, 0);
	Get().lua_pcall(*L, 0, 0, 0);
}

void PaydayLua::HookLuaClose(lua_State* L)
{
	Get().OnCloseState(L);
	inGame.tLuaClose(L);
}

void PaydayLua::HookRequire(lua_State* L)
{
	auto&& lua = Get();
	lua.lua_getfield(L, LUA_GLOBALSINDEX, "require");
	SafeCall(inGame.lua_pushcclosure, L, [](auto L) {
		auto&& lua = Get();
		std::string name = lua.lua_tolstring(L, 1, nullptr);
		lua.lua_pushvalue(L, lua_upvalueindex(1));
		lua.lua_insert(L, 1);
		lua.lua_call(L, lua.lua_gettop(L) - 1, LUA_MULTRET);
		SafeCall(inGame.lua_pushcclosure, L, [](auto L) {
			auto name = static_cast<std::string*>(Get().lua_touserdata(L, 1));
			return Get().OnRequire(L, *name), 0;
		}, 0);
		lua.lua_pushlightuserdata(L, &name);
		lua.lua_call(L, 1, 0);
		return lua.lua_gettop(L);
	}, 1);
	lua.lua_setfield(L, LUA_GLOBALSINDEX, "require");
}

int PaydayLua::luaL_loadfile(lua_State* L, const char* filename)
{
	return SafeCall(inGame.luaL_loadfile, L, filename);
}

int PaydayLua::luaL_loadstring(lua_State* L, const char *s)
{
	return SafeCall(inGame.luaL_loadbuffer, L, s, strlen(s), s);
}

int PaydayLua::luaL_newmetatable(lua_State* L, const char* tname)
{
	lua_getfield(L, LUA_REGISTRYINDEX, tname);  /* get registry.name */
	if (!lua_isnil(L, -1))  /* name already in use? */
		return 0;  /* leave previous value on top, but return 0 */
	lua_pop(L, 1);
	lua_newtable(L);  /* create metatable */
	lua_pushvalue(L, -1);
	lua_setfield(L, LUA_REGISTRYINDEX, tname);  /* registry.name = metatable */
	return 1;
}

int PaydayLua::luaL_ref(lua_State* L, int t)
{
	if (!(t > 0 || t <= LUA_REGISTRYINDEX))
		t += lua_gettop(L) + 1;
	if (lua_isnil(L, -1))
		return lua_pop(L, 1), LUA_REFNIL;
	lua_rawgeti(L, t, 0);
	auto ref = lua_tointeger(L, -1);
	lua_pop(L, 1);
	if (ref != 0) {
		lua_rawgeti(L, t, ref);
		lua_rawseti(L, t, 0);
	}
	else
		ref = 1 + lua_objlen(L, t);
	lua_rawseti(L, t, ref);
	return ref;
}

void PaydayLua::luaL_unref(lua_State* L, int t, int ref)
{
	return SafeCall(inGame.luaL_unref, L, t, ref);
}

void PaydayLua::lua_call(lua_State* L, int nargs, int nresults)
{
	auto func = L->top - (nargs + 1);
	SafeCall(inGame.luaD_call, L, func, nresults);
	if (nresults == LUA_MULTRET && L->top >= L->ci->top)
		L->ci->top = L->top;
}

int PaydayLua::lua_checkstack(lua_State* L, int size)
{
	return Lua::lua_checkstack(L, size);
}

void PaydayLua::lua_close(lua_State* L)
{
	SafeCall(inGame.lua_close, L);
}

void PaydayLua::lua_concat(lua_State* L, int n)
{
	Lua::lua_concat(L, n);
}

void PaydayLua::lua_createtable(lua_State* L, int narr, int nrec)
{
	SafeCall(inGame.lua_createtable, L, narr, nrec);
}

int PaydayLua::lua_dump(lua_State* L, lua_Writer writer, void* data)
{
	return Lua::lua_dump(L, writer, data);
}

int PaydayLua::lua_equal(lua_State* L, int idx1, int idx2)
{
	return Lua::lua_equal(L, idx1, idx2);
}

int PaydayLua::lua_error(lua_State* L)
{
	SafeCall(inGame.luaG_errormsg, L);
	return 0;
}

void PaydayLua::lua_getfield(lua_State* L, int idx, const char* name)
{
	SafeCall(inGame.lua_getfield, L, idx, name);
}

int PaydayLua::lua_getinfo(lua_State* L, const char* s, lua_Debug* debug)
{
	return Lua::lua_getinfo(L, s, debug);
}

int PaydayLua::lua_getmetatable(lua_State* L, int index)
{
	return Lua::lua_getmetatable(L, index);
}

int PaydayLua::lua_getstack(lua_State* L, int level, lua_Debug* ar)
{
	return Lua::lua_getstack(L, level, ar);
}

void PaydayLua::lua_gettable(lua_State* L, int index)
{
	SafeCall(inGame.lua_gettable, L, index);
}

int PaydayLua::lua_gettop(lua_State* L)
{
	return Lua::lua_gettop(L);
}

void PaydayLua::lua_insert(lua_State* L, int index)
{
	Lua::lua_insert(L, index);
}

int PaydayLua::lua_isnumber(lua_State* L, int index)
{
	return Lua::lua_isnumber(L, index);
}

int PaydayLua::lua_lessthan(lua_State* L, int idx1, int idx2)
{
	return Lua::lua_lessthan(L, idx1, idx2);
}

int PaydayLua::lua_load(lua_State* L, lua_Reader reader, void* data,
						const char* chunkname)
{
	ZIO z{ 0, nullptr, reader, data, L };
	if (!chunkname)
		chunkname = "?";
	return SafeCall(inGame.luaD_protectedparser, L, &z, chunkname);
}

void* PaydayLua::lua_newuserdata(lua_State *L, size_t len)
{
	return SafeCall(inGame.lua_newuserdata, L, len);
}

lua_State* PaydayLua::lua_newthread(lua_State* L)
{
	if (G(L)->gcstepmul)
		G(L)->gcstepmul--, SafeCall(inGame.luaC_fullgc, L);
	if (G(L)->totalbytes >= G(L)->GCthreshold)
		SafeCall(inGame.luaC_step, L);
	auto L1 = SafeCall(inGame.luaE_newthread, L);
	setthvalue(L, L->top++, L1);
	return L1;
}

int PaydayLua::lua_next(lua_State* L, int index)
{
	return Lua::lua_next(L, index);
}

size_t PaydayLua::lua_objlen(lua_State* L, int index)
{
	return SafeCall(inGame.lua_objlen, L, index);
}

int PaydayLua::lua_pcall(lua_State* L, int nargs, int nresults, int errfunc)
{
	return SafeCall(inGame.lua_pcall, L, nargs, nresults, errfunc);
}

void PaydayLua::lua_pushboolean(lua_State* L, int b)
{
	Lua::lua_pushboolean(L, b);
}

void PaydayLua::lua_pushcclosure(lua_State* L, lua_CPPFunction func, int nups)
{
	lua_pushlightuserdata(L, func);
	SafeCall(inGame.lua_pushcclosure, L, [](auto L) {
		auto&& lua = Get();
		lua_Debug info;
		lua.lua_getstack(L, 0, &info);
		auto funcPtr = lua.lua_touserdata(L, lua_upvalueindex(info.nups));
		auto func = static_cast<lua_CPPFunction>(funcPtr);
		return func(L, lua);
	}, nups + 1);
}

const char* PaydayLua::lua_pushfstring(lua_State* L, const char* s, ...)
{
	va_list va;
	va_start(va, s);
	auto ret = lua_pushvfstring(L, s, va);
	va_end(va);
	return ret;
}

void PaydayLua::lua_pushinteger(lua_State* L, lua_Integer n)
{
	Lua::lua_pushinteger(L, n);
}

void PaydayLua::lua_pushlightuserdata(lua_State* L, void* p)
{
	Lua::lua_pushlightuserdata(L, p);
}

void PaydayLua::lua_pushlstring(lua_State* L, const char* str, size_t len)
{
	SafeCall(inGame.lua_pushlstring, L, str, len);
}

void PaydayLua::lua_pushnil(lua_State* L)
{
	Lua::lua_pushnil(L);
}

void PaydayLua::lua_pushnumber(lua_State* L, lua_Number n)
{
	Lua::lua_pushnumber(L, n);
}

void PaydayLua::lua_pushstring(lua_State* L, const char* s)
{
	s != nullptr ? lua_pushlstring(L, s, strlen(s)) : lua_pushnil(L);
}

int PaydayLua::lua_pushthread(lua_State* L)
{
	return Lua::lua_pushthread(L);
}

void PaydayLua::lua_pushvalue(lua_State* L, int index)
{
	Lua::lua_pushvalue(L, index);
}

const char* PaydayLua::lua_pushvfstring(lua_State* L, const char* s, va_list v)
{
	if (G(L)->gcstepmul)
		G(L)->gcstepmul--, SafeCall(inGame.luaC_fullgc, L);
	if (G(L)->totalbytes >= G(L)->GCthreshold)
		SafeCall(inGame.luaC_step, L);
	return SafeCall(inGame.luaO_pushvfstring, L, s, v);
}

int PaydayLua::lua_rawequal(lua_State* L, int idx1, int idx2)
{
	return Lua::lua_rawequal(L, idx1, idx2);
}

void PaydayLua::lua_rawget(lua_State* L, int index)
{
	Lua::lua_rawget(L, index);
}

void PaydayLua::lua_rawgeti(lua_State* L, int index, int n)
{
	SafeCall(inGame.lua_rawgeti, L, index, n);
}

void PaydayLua::lua_rawset(lua_State* L, int index)
{
	SafeCall(inGame.lua_rawset, L, index);
}

void PaydayLua::lua_rawseti(lua_State* L, int index, int n)
{
	SafeCall(inGame.lua_rawseti, L, index, n);
}

void PaydayLua::lua_remove(lua_State* L, int index)
{
	Lua::lua_remove(L, index);
}

void PaydayLua::lua_replace(lua_State* L, int index)
{
	Lua::lua_replace(L, index);
}

int PaydayLua::lua_resume(lua_State* L, int narg)
{
	return Lua::lua_resume(L, narg);
}

void PaydayLua::lua_setfield(lua_State* L, int index, const char* k)
{
	SafeCall(inGame.lua_setfield, L, index, k);
}

int PaydayLua::lua_setmetatable(lua_State* L, int index)
{
	return Lua::lua_setmetatable(L, index);
}

void PaydayLua::lua_settable(lua_State* L, int index)
{
	auto t = Lua::index2adr(L, index);
	SafeCall(inGame.luaV_settable, L, t, L->top - 2, L->top - 1);
	L->top -= 2;
}

void PaydayLua::lua_settop(lua_State* L, int n)
{
	Lua::lua_settop(L, n);
}

int PaydayLua::lua_toboolean(lua_State* L, int index)
{
	return Lua::lua_toboolean(L, index);
}

lua_CFunction PaydayLua::lua_tocfunction(lua_State* L, int index)
{
	return Lua::lua_tocfunction(L, index);
}

int PaydayLua::lua_tointeger(lua_State* L, int index)
{
	return Lua::lua_tointeger(L, index);
}

const char* PaydayLua::lua_tolstring(lua_State* L, int index, size_t* len)
{
	return SafeCall(inGame.lua_tolstring, L, index, len);
}

lua_Number PaydayLua::lua_tonumber(lua_State* L, int index)
{
	return Lua::lua_tonumber(L, index);
}

lua_State* PaydayLua::lua_tothread(lua_State* L, int index)
{
	return Lua::lua_tothread(L, index);
}

void* PaydayLua::lua_touserdata(lua_State* L, int index)
{
	return Lua::lua_touserdata(L, index);
}

int PaydayLua::lua_type(lua_State* L, int index)
{
	return Lua::lua_type(L, index);
}

const char* PaydayLua::lua_typename(lua_State* L, int tp)
{
	return Lua::lua_typename(L, tp);
}

void PaydayLua::lua_xmove(lua_State* from, lua_State* to, int n)
{
	Lua::lua_xmove(from, to, n);
}

int PaydayLua::lua_yield(lua_State* L, int nresults)
{
	return Lua::lua_yield(L, nresults);
}
