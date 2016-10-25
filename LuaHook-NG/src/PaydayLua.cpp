#include "stdafx.h"
#include "FunctionHooker.h"
#include "LuaGameImpl.h"
#include "LuaHookNG.h"
#include "PaydayLua.h"
#include "ProcessSearcher.h"
#include "TempDetour.h"
#include "Utility.h"

using namespace Olipro;

PaydayLua& PaydayLua::Get()
{
	static PaydayLua self;
	return self;
}

PaydayLua::PaydayLua() : inGame{ InGameHandling::GetInstance(ProcessGameTick,
	ProcessNewState, ProcessCloseState) }
{
	
}

void PaydayLua::ProcessGameTick(lua_State* L, const char* op)
{
	if (Str2Uint(op) == Str2Uint("update")) {
		auto&& self = Get();
		SafeCall(self.inGame.functions.lua_pushcclosure, L, [](auto L) {
			return Get().OnGameTick(L), 0;
		}, 0);
		self.lua_pcall(L, 0, 0, 0);
	}
}

void PaydayLua::ProcessNewState(lua_State* L)
{
	auto&& self = Get();
	self.packageLoaded.clear();
	self.OnNewState(L);
	HookRequire(L);
	self.lua_pcall(L, 0, 0, 0);
}

void PaydayLua::ProcessCloseState(lua_State* L)
{
	Get().OnCloseState(L);
}

void PaydayLua::HookRequire(lua_State* L)
{
	auto&& lua = Get();
	lua.lua_getfield(L, LUA_GLOBALSINDEX, "require");
	SafeCall(lua.inGame.functions.lua_pushcclosure, L, [](auto L) {
		auto&& lua = Get();
		std::string name = lua.lua_tolstring(L, 1, nullptr);
		lua.lua_pushvalue(L, lua_upvalueindex(1));
		lua.lua_insert(L, 1);
		lua.lua_call(L, lua.lua_gettop(L) - 1, LUA_MULTRET);
		if (lua.packageLoaded.find(name) == lua.packageLoaded.end()) {
			lua.packageLoaded.insert(name);
			SafeCall(lua.inGame.functions.lua_pushcclosure, L, [](auto L) {
				auto&& name = *static_cast<std::string*>(
					Get().lua_touserdata(L, 1));
				return Get().OnRequire(L, name), 0;
			}, 0);
			lua.lua_pushlightuserdata(L, &name);
			lua.lua_call(L, 1, 0);
		}
		return lua.lua_gettop(L);
	}, 1);
	lua.lua_setfield(L, LUA_GLOBALSINDEX, "require");
}

int PaydayLua::luaL_error(lua_State* L, const char* fmt, ...)
{
	//Compiler hack - if a __cdecl takes no params, it won't adjust the stack
	return Get().inGame.functions.luaL_error();
}

int PaydayLua::luaL_loadbuffer(lua_State* L, const char* buf, size_t sz,
	const char* name)
{
	return SafeCall(inGame.functions.luaL_loadbuffer, L, buf, sz, name);
}

int PaydayLua::luaL_loadfile(lua_State* L, const char* filename)
{
	return SafeCall(inGame.functions.luaL_loadfile, L, filename);
}

int PaydayLua::luaL_loadstring(lua_State* L, const char *s)
{
	return luaL_loadbuffer(L, s, strlen(s), s);
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

void PaydayLua::luaL_openlib(lua_State* L, const char* libname,
	const luaL_Reg* l, int nup)
{
	if (libname) {
		int size = 0;
		while (l++->name)
			++size;
		SafeCall(inGame.functions.luaL_findtable, L, LUA_REGISTRYINDEX,
			"_LOADED", size);
			lua_getfield(L, -1, libname);
			if (!lua_istable(L, -1)) {
				lua_pop(L, 1);
				if (SafeCall(inGame.functions.luaL_findtable, L,
					LUA_GLOBALSINDEX, libname, size) != NULL)
					luaL_error(L, "name conflict for module " LUA_QS, libname);
				lua_pushvalue(L, -1);
				lua_setfield(L, -3, libname);
			}
			lua_remove(L, -2);
			lua_insert(L, -(nup + 1));
	}
	for (; l->name; l++) {
		for (int i = 0; i < nup; i++)
			lua_pushvalue(L, -nup);
		SafeCall(inGame.functions.lua_pushcclosure, L, l->func, nup);
		lua_setfield(L, -(nup + 2), l->name);
	}
	lua_pop(L, nup);
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
	const int freelistRef = 0;
	if (ref >= 0) {
		t = t > 0 || t <= LUA_REGISTRYINDEX ? t : lua_gettop(L) + t + 1;
		lua_rawgeti(L, t, freelistRef);
		lua_rawseti(L, t, ref);
		lua_pushinteger(L, ref);
		lua_rawseti(L, t, freelistRef);
	}
}

void PaydayLua::lua_call(lua_State* L, int nargs, int nresults)
{
	auto func = L->top - (nargs + 1);
	SafeCall(inGame.functions.luaD_call, L, func, nresults);
	if (nresults == LUA_MULTRET && L->top >= L->ci->top)
		L->ci->top = L->top;
}

int PaydayLua::lua_checkstack(lua_State* L, int size)
{
	return SafeCall(inGame.functions.lua_checkstack, L, size);
}

void PaydayLua::lua_close(lua_State* L)
{
	SafeCall(inGame.functions.lua_close, L);
}

void PaydayLua::lua_concat(lua_State* L, int n)
{
	Lua::lua_concat(L, n);
}

void PaydayLua::lua_createtable(lua_State* L, int narr, int nrec)
{
	SafeCall(inGame.functions.lua_createtable, L, narr, nrec);
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
	SafeCall(inGame.functions.luaG_errormsg, L);
	return 0;
}

int PaydayLua::lua_gc(lua_State* L, int what, int data)
{
	return 0; //removed from PAYDAY 2 as of 2016-10-10
}

void PaydayLua::lua_getfield(lua_State* L, int idx, const char* name)
{
	SafeCall(inGame.functions.lua_getfield, L, idx, name);
}

lua_Hook PaydayLua::lua_gethook(lua_State* L)
{
	return Lua::lua_gethook(L);
}

int PaydayLua::lua_gethookcount(lua_State* L)
{
	return Lua::lua_gethookcount(L);
}

int PaydayLua::lua_gethookmask(lua_State* L)
{
	return Lua::lua_gethookmask(L);
}

int PaydayLua::lua_getinfo(lua_State* L, const char* s, lua_Debug* debug)
{
	return SafeCall(inGame.functions.lua_getinfo, L, s, debug);
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
	SafeCall(inGame.functions.lua_gettable, L, index);
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
	return SafeCall(inGame.functions.luaD_protectedparser, L, &z, chunkname);
}

void* PaydayLua::lua_newuserdata(lua_State *L, size_t len)
{
	return SafeCall(inGame.functions.lua_newuserdata, L, len);
}

lua_State* PaydayLua::lua_newthread(lua_State* L)
{
	if (G(L)->gcstepmul)
		G(L)->gcstepmul--, SafeCall(inGame.functions.luaC_fullgc, L);
	if (G(L)->totalbytes >= G(L)->GCthreshold)
		SafeCall(inGame.functions.luaC_step, L);
	auto L1 = SafeCall(inGame.functions.luaE_newthread, L);
	setthvalue(L, L->top++, L1);
	return L1;
}

int PaydayLua::lua_next(lua_State* L, int index)
{
	return Lua::lua_next(L, index);
}

size_t PaydayLua::lua_objlen(lua_State* L, int index)
{
	return SafeCall(inGame.functions.lua_objlen, L, index);
}

int PaydayLua::lua_pcall(lua_State* L, int nargs, int nresults, int errfunc)
{
	return SafeCall(inGame.functions.lua_pcall, L, nargs, nresults, errfunc);
}

void PaydayLua::lua_pushboolean(lua_State* L, int b)
{
	Lua::lua_pushboolean(L, b);
}

void PaydayLua::lua_pushcclosure(lua_State* L, lua_CPPFunction func, int nups)
{
	lua_pushlightuserdata(L, reinterpret_cast<void*>(func));
	lua_insert(L, -nups - 1);
	lua_pushinteger(L, nups);
	lua_insert(L, -nups - 1);
	SafeCall(inGame.functions.lua_pushcclosure, L, [](auto L) {
		auto&& lua = Get();
		auto nups = lua.lua_tointeger(L, lua_upvalueindex(2));
		auto narg = lua.lua_gettop(L);
		for (auto i = 1; i <= nups; i++)
			lua.lua_pushvalue(L, lua_upvalueindex(i+2));
		SafeCall(lua.inGame.functions.lua_pushcclosure, L, [](auto L) {
			auto&& lua = Get();
			auto f = reinterpret_cast<lua_CPPFunction>(
				lua.lua_touserdata(L, -1));
			lua.lua_remove(L, -1);
			lua.yieldCheck = {};
			auto ret = f(L, lua);
			return !lua.yieldCheck.isYield ? ret : lua.yieldCheck.nargs;
		}, nups);
		lua.lua_insert(L, 1);
		lua.lua_pushvalue(L, lua_upvalueindex(1));
		lua.lua_call(L, narg + 1, LUA_MULTRET);
		return !lua.yieldCheck.isYield ? lua.lua_gettop(L) :
									Lua::lua_yield(L, lua.yieldCheck.nargs);
	}, nups + 2);
}

void PaydayLua::lua_pushrawcclosure(lua_State* L, lua_CFunction func, int nups)
{
	SafeCall(inGame.functions.lua_pushcclosure, L, func, nups);
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
	SafeCall(inGame.functions.lua_pushlstring, L, str, len);
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
		G(L)->gcstepmul--, SafeCall(inGame.functions.luaC_fullgc, L);
	if (G(L)->totalbytes >= G(L)->GCthreshold)
		SafeCall(inGame.functions.luaC_step, L);
	return SafeCall(inGame.functions.luaO_pushvfstring, L, s, v);
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
	SafeCall(inGame.functions.lua_rawgeti, L, index, n);
}

void PaydayLua::lua_rawset(lua_State* L, int index)
{
	SafeCall(inGame.functions.lua_rawset, L, index);
}

void PaydayLua::lua_rawseti(lua_State* L, int index, int n)
{
	SafeCall(inGame.functions.lua_rawseti, L, index, n);
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
	return SafeCall(inGame.functions.lua_resume, L, narg);
}

void PaydayLua::lua_setfield(lua_State* L, int index, const char* k)
{
	SafeCall(inGame.functions.lua_setfield, L, index, k);
}

int PaydayLua::lua_sethook(lua_State* L, lua_Hook f, int mask, int count)
{
	return Lua::lua_sethook(L, f, mask, count);
}

int PaydayLua::lua_setmetatable(lua_State* L, int index)
{
	return Lua::lua_setmetatable(L, index);
}

void PaydayLua::lua_settable(lua_State* L, int index)
{
	auto t = Lua::index2adr(L, index);
	SafeCall(inGame.functions.luaV_settable, L, t, L->top - 2, L->top - 1);
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
	return SafeCall(inGame.functions.lua_tolstring, L, index, len);
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
	yieldCheck = { true, nresults };
	return -1;
}
