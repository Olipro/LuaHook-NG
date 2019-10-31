#pragma once

#include "LuaAPI.h"

namespace LuaHookNG {
	struct LuaFuncPtrs {
		static inline LuaFuncPtrs* inst;
		decltype(lua_newstate)& lua_newstate_;
		decltype(lua_close)& lua_close_;
		decltype(lua_newthread)& lua_newthread_;

		decltype(lua_gettop)& lua_gettop_;
		decltype(lua_settop)& lua_settop_;
		decltype(lua_pushvalue)& lua_pushvalue_;
		decltype(lua_remove)& lua_remove_;
		decltype(lua_insert)& lua_insert_;
		decltype(lua_replace)& lua_replace_;
		decltype(lua_checkstack)& lua_checkstack_;

		decltype(lua_xmove)& lua_xmove_;

		decltype(lua_isnumber)& lua_isnumber_;
		decltype(lua_isstring)& lua_isstring_;
		decltype(lua_iscfunction)& lua_iscfunction_;
		decltype(lua_isuserdata)& lua_isuserdata_;
		decltype(lua_type)& lua_type_;
		decltype(lua_typename)& lua_typename_;

		decltype(lua_equal)& lua_equal_;
		decltype(lua_rawequal)& lua_rawequal_;
		decltype(lua_lessthan)& lua_lessthan_;
		decltype(lua_tonumber)& lua_tonumber_;
		decltype(lua_tointeger)& lua_tointeger_;
		decltype(lua_toboolean)& lua_toboolean_;
		decltype(lua_tolstring)& lua_tolstring_;
		decltype(lua_objlen)& lua_objlen_;
		decltype(lua_tocfunction)& lua_tocfunction_;
		decltype(lua_touserdata)& lua_touserdata_;
		decltype(lua_tothread)& lua_tothread_;
		decltype(lua_topointer)& lua_topointer_;

		decltype(lua_pushnil)& lua_pushnil_;
		decltype(lua_pushnumber)& lua_pushnumber_;
		decltype(lua_pushinteger)& lua_pushinteger_;
		decltype(lua_pushlstring)& lua_pushlstring_;
		decltype(lua_pushstring)& lua_pushstring_;
		decltype(lua_pushvfstring)& lua_pushvfstring_;
		decltype(lua_pushcclosure)& lua_pushcclosure_;
		decltype(lua_pushboolean)& lua_pushboolean_;
		decltype(lua_pushlightuserdata)& lua_pushlightuserdata_;
		decltype(lua_pushthread)& lua_pushthread_;

		decltype(lua_gettable)& lua_gettable_;
		decltype(lua_getfield)& lua_getfield_;
		decltype(lua_rawget)& lua_rawget_;
		decltype(lua_rawgeti)& lua_rawgeti_;
		decltype(lua_createtable)& lua_createtable_;
		decltype(lua_newuserdata)& lua_newuserdata_;
		decltype(lua_getmetatable)& lua_getmetatable_;
		decltype(lua_getfenv)& lua_getfenv_;

		decltype(lua_settable)& lua_settable_;
		decltype(lua_setfield)& lua_setfield_;
		decltype(lua_rawset)& lua_rawset_;
		decltype(lua_rawseti)& lua_rawseti_;
		decltype(lua_setmetatable)& lua_setmetatable_;
		decltype(lua_setfenv)& lua_setfenv_;

		decltype(lua_call)& lua_call_;
		decltype(lua_pcall)& lua_pcall_;
		decltype(lua_cpcall)& lua_cpcall_;
		decltype(lua_load)& lua_load_;

		decltype(lua_dump)& lua_dump_;

		decltype(lua_yield)& lua_yield_;
		decltype(lua_resume)& lua_resume_;
		decltype(lua_status)& lua_status_;

		decltype(lua_next)& lua_next_;
		decltype(lua_concat)& lua_concat_;

		decltype(luaL_ref)& luaL_ref_;
		decltype(luaL_unref)& luaL_unref_;
		decltype(luaL_loadfile)& luaL_loadfile_;
		decltype(luaL_loadbuffer)& luaL_loadbuffer_;
		decltype(luaL_loadstring)& luaL_loadstring_;
		decltype(luaL_newmetatable)& luaL_newmetatable_;
		decltype(luaL_newstate)& luaL_newstate_;
	};
}  // namespace LuaHookNG
