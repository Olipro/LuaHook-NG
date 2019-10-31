#pragma once

#include <cstddef>

#include "LinkSpec.h"

namespace LuaHookNG {
	struct lua_State;
	using lua_Alloc		= void* (*)(void* ud,
								void* ptr,
								size_t osize,
								size_t nsize);
	using lua_Number	= double;
	using lua_Integer	= ptrdiff_t;
	using lua_CFunction = int (*)(lua_State*);
	using lua_Reader	= const char* (*)(lua_State*, void* ud, size_t* sz);
	using lua_Writer = int (*)(lua_State*, const void* p, size_t sz, void* ud);
	static constexpr int LUA_GLOBALSINDEX	= -10002;
	static constexpr int LUA_MULTRET		= -1;
	static constexpr int LUA_TNIL			= 0;
	static constexpr int LUA_TBOOLEAN		= 1;
	static constexpr int LUA_TLIGHTUSERDATA = 2;
	static constexpr int LUA_TNUMBER		= 3;
	static constexpr int LUA_TSTRING		= 4;
	static constexpr int LUA_TTABLE			= 5;
	static constexpr int LUA_TFUNCTION		= 6;
	static constexpr int LUA_TUSERDATA		= 7;
	static constexpr int LUA_TTHREAD		= 8;
	static constexpr int LUA_REGISTRYINDEX	= -10000;

	extern "C" {
	DLLSPEC lua_State* lua_newstate(lua_Alloc, void*);
	DLLSPEC void lua_close(lua_State*);
	DLLSPEC lua_State* lua_newthread(lua_State*);

	DLLSPEC int lua_gettop(lua_State*);
	DLLSPEC void lua_settop(lua_State*, int);
	DLLSPEC void lua_pushvalue(lua_State*, int);
	DLLSPEC void lua_remove(lua_State*, int);
	DLLSPEC void lua_insert(lua_State*, int);
	DLLSPEC void lua_replace(lua_State*, int);
	DLLSPEC int lua_checkstack(lua_State*, int);

	DLLSPEC void lua_xmove(lua_State* from, lua_State* to, int n);

	DLLSPEC int lua_isnumber(lua_State*, int);
	DLLSPEC int lua_isstring(lua_State*, int);
	DLLSPEC int lua_iscfunction(lua_State*, int);
	DLLSPEC int lua_isuserdata(lua_State*, int);
	DLLSPEC int lua_type(lua_State*, int);
	DLLSPEC const char* lua_typename(lua_State*, int);

	DLLSPEC int lua_equal(lua_State*, int, int);
	DLLSPEC int lua_rawequal(lua_State*, int, int);
	DLLSPEC int lua_lessthan(lua_State*, int, int);
	DLLSPEC lua_Number lua_tonumber(lua_State*, int);
	DLLSPEC lua_Integer lua_tointeger(lua_State*, int);
	DLLSPEC int lua_toboolean(lua_State*, int);
	DLLSPEC const char* lua_tolstring(lua_State*, int, size_t*);
	DLLSPEC size_t lua_objlen(lua_State*, int);
	DLLSPEC lua_CFunction lua_tocfunction(lua_State*, int);
	DLLSPEC void* lua_touserdata(lua_State*, int);
	DLLSPEC lua_State* lua_tothread(lua_State*, int);
	DLLSPEC const void* lua_topointer(lua_State*, int);

	DLLSPEC void lua_pushnil(lua_State*);
	DLLSPEC void lua_pushnumber(lua_State*, lua_Number);
	DLLSPEC void lua_pushinteger(lua_State*, lua_Integer);
	DLLSPEC void lua_pushlstring(lua_State*, const char*, size_t);
	DLLSPEC void lua_pushstring(lua_State*, const char*);
	DLLSPEC const char* lua_pushvfstring(lua_State*, const char* fmt, va_list);
	DLLSPEC const char* lua_pushfstring(lua_State*, const char* fmt, ...);
	DLLSPEC void lua_pushcclosure(lua_State*, lua_CFunction, int n);
	DLLSPEC void lua_pushboolean(lua_State*, int b);
	DLLSPEC void lua_pushlightuserdata(lua_State*, void*);
	DLLSPEC int lua_pushthread(lua_State*);

	DLLSPEC void lua_gettable(lua_State*, int);
	DLLSPEC void lua_getfield(lua_State*, int, const char*);
	DLLSPEC void lua_rawget(lua_State*, int);
	DLLSPEC void lua_rawgeti(lua_State*, int idx, int n);
	DLLSPEC void lua_createtable(lua_State*, int narr, int nrec);
	DLLSPEC void* lua_newuserdata(lua_State*, size_t);
	DLLSPEC int lua_getmetatable(lua_State*, int);
	DLLSPEC void lua_getfenv(lua_State*, int);

	DLLSPEC void lua_settable(lua_State*, int);
	DLLSPEC void lua_setfield(lua_State*, int, const char*);
	DLLSPEC void lua_rawset(lua_State*, int);
	DLLSPEC void lua_rawseti(lua_State*, int idx, int n);
	DLLSPEC void lua_setmetatable(lua_State*, int);
	DLLSPEC void lua_setfenv(lua_State*, int);

	DLLSPEC void lua_call(lua_State*, int nargs, int nresults);
	DLLSPEC int lua_pcall(lua_State*, int nargs, int nresults, int errfunc);
	DLLSPEC int lua_cpcall(lua_State*, lua_CFunction func, void* ud);
	DLLSPEC int lua_load(lua_State*,
						 lua_Reader,
						 void* dt,
						 const char* chunkname);

	DLLSPEC int lua_dump(lua_State*, lua_Writer, void* data);

	DLLSPEC int lua_yield(lua_State*, int nresults);
	DLLSPEC int lua_resume(lua_State*, int narg);
	DLLSPEC int lua_status(lua_State*);

	DLLSPEC int lua_next(lua_State*, int);
	DLLSPEC void lua_concat(lua_State*, int);

	DLLSPEC int luaL_ref(lua_State*, int t);
	DLLSPEC void luaL_unref(lua_State*, int t, int ref);
	DLLSPEC int luaL_loadfile(lua_State*, const char* filename);
	DLLSPEC int luaL_loadbuffer(lua_State*,
								const char* buf,
								size_t sz,
								const char* name);
	DLLSPEC int luaL_loadstring(lua_State*, const char*);
	DLLSPEC int luaL_newmetatable(lua_State*, const char*);
	DLLSPEC lua_State* luaL_newstate();
	}

	// Convenience wrappers.
	inline void lua_getglobal(lua_State* L, const char* name) {
		lua_getfield(L, LUA_GLOBALSINDEX, name);
	}
	inline void lua_setglobal(lua_State* L, const char* name) {
		lua_setfield(L, LUA_GLOBALSINDEX, name);
	}
	inline int lua_upvalueindex(int n) {
		return LUA_GLOBALSINDEX - n;
	}
	inline void lua_pushcfunction(lua_State* L, lua_CFunction f) {
		lua_pushcclosure(L, f, 0);
	}
	inline void lua_pop(lua_State* L, int n) {
		lua_settop(L, -(n)-1);
	}
	inline const char* lua_tostring(lua_State* L, int n) {
		return lua_tolstring(L, n, nullptr);
	}
}  // namespace LuaHookNG
