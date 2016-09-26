#pragma once
#include <string>
#include "Lua.h"
#include "lauxlib.h"

namespace Olipro {

	class LuaInterface {
	protected:
		void OnGameTick(lua_State*);
		void OnNewState(lua_State*);
		void OnCloseState(lua_State*);
		void OnRequire(lua_State*, const std::string&);
	public:
		typedef int(*lua_CPPFunction)(lua_State*, LuaInterface&);
		//Lua C API Functions
		virtual int luaL_error(lua_State*, const char* fmt, ...) = 0;
		virtual int luaL_loadbuffer(lua_State*, const char*, size_t,
									const char*) = 0;
		virtual int luaL_loadfile(lua_State*, const char*) = 0;
		virtual int luaL_loadstring(lua_State*, const char*) = 0;
		virtual int luaL_newmetatable(lua_State*, const char*) = 0;
		virtual void luaL_openlib(	lua_State*, const char*, const luaL_Reg*,
									int) = 0;
		virtual int luaL_ref(lua_State*, int) = 0;
		virtual void luaL_unref(lua_State*, int, int) = 0;
		virtual void lua_call(lua_State*, int, int) = 0;
		virtual int lua_checkstack(lua_State *, int) = 0;
		virtual void lua_close(lua_State*) = 0;
		virtual void lua_concat(lua_State*, int) = 0;
		virtual void lua_createtable(lua_State *, int, int) = 0;
		virtual int lua_dump(lua_State*, lua_Writer, void*) = 0;
		virtual int lua_equal(lua_State *, int, int) = 0;
		virtual int lua_error(lua_State*) = 0;
		virtual int lua_gc(lua_State*, int, int) = 0;
		virtual void lua_getfield(lua_State*, int, const char*) = 0;
		virtual lua_Hook lua_gethook(lua_State*) = 0;
		virtual int lua_gethookcount(lua_State*) = 0;
		virtual int lua_gethookmask(lua_State*) = 0;
		virtual int lua_getinfo(lua_State*, const char*, lua_Debug*) = 0;
		virtual int lua_getmetatable(lua_State *, int) = 0;
		virtual int lua_getstack(lua_State*, int, lua_Debug*) = 0;
		virtual void lua_gettable(lua_State*, int) = 0;
		virtual int lua_gettop(lua_State*) = 0;
		virtual void lua_insert(lua_State*, int) = 0;
		virtual int lua_isnumber(lua_State*, int) = 0;
		virtual int lua_lessthan(lua_State*, int, int) = 0;
		virtual int lua_load(lua_State*, lua_Reader, void*, const char*) = 0;
		virtual void* lua_newuserdata(lua_State*, size_t) = 0;
		virtual lua_State* lua_newthread(lua_State*) = 0;
		virtual int lua_next(lua_State*, int) = 0;
		virtual size_t lua_objlen(lua_State*, int) = 0;
		virtual int lua_pcall(lua_State*, int, int, int) = 0;
		virtual void lua_pushboolean(lua_State*, int) = 0;
		virtual void lua_pushcclosure(lua_State*, lua_CPPFunction, int) = 0;
		virtual const char* lua_pushfstring(lua_State*, const char *, ...) = 0;
		virtual void lua_pushinteger(lua_State*, lua_Integer) = 0;
		virtual void lua_pushlightuserdata(lua_State*, void*) = 0;
		virtual void lua_pushlstring(lua_State*, const char*, size_t) = 0;
		virtual void lua_pushnil(lua_State*) = 0;
		virtual void lua_pushnumber(lua_State*, lua_Number) = 0;
		virtual void lua_pushstring(lua_State*, const char*) = 0;
		virtual int lua_pushthread(lua_State*) = 0;
		virtual void lua_pushvalue(lua_State*, int) = 0;
		virtual const char* lua_pushvfstring(	lua_State*, const char*,
												va_list) = 0;
		virtual int lua_rawequal(lua_State*, int, int) = 0;
		virtual void lua_rawget(lua_State*, int) = 0;
		virtual void lua_rawgeti(lua_State*, int, int) = 0;
		virtual void lua_rawset(lua_State*, int) = 0;
		virtual void lua_rawseti(lua_State*, int, int) = 0;
		virtual void lua_remove(lua_State*, int) = 0;
		virtual void lua_replace(lua_State*, int) = 0;
		virtual int lua_resume(lua_State*, int) = 0;
		virtual void lua_setfield(lua_State*, int, const char*) = 0;
		virtual int lua_sethook(lua_State*, lua_Hook, int, int) = 0;
		virtual int lua_setmetatable(lua_State*, int) = 0;
		virtual void lua_settable(lua_State*, int) = 0;
		virtual void lua_settop(lua_State*, int) = 0;
		virtual int lua_tointeger(lua_State*, int) = 0;
		virtual const char* lua_tolstring(lua_State*, int, size_t*) = 0;
		virtual int lua_toboolean(lua_State*, int) = 0;
		virtual lua_CFunction lua_tocfunction(lua_State*, int) = 0;
		virtual lua_Number lua_tonumber(lua_State*, int) = 0;
		virtual lua_State* lua_tothread(lua_State*, int) = 0;
		virtual void* lua_touserdata(lua_State*, int) = 0;
		virtual int lua_type(lua_State*, int) = 0;
		virtual const char* lua_typename(lua_State*, int) = 0;
		virtual void lua_xmove(lua_State*, lua_State*, int) = 0;
		virtual int lua_yield(lua_State*, int) = 0;

		virtual ~LuaInterface() = default;
	};
}
