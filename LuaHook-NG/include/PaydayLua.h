#pragma once
#include "LuaInterface.h"
#include "PaydayLua/InGameWin32.h"

namespace Olipro {

	class PaydayLua final : public LuaInterface
	{
		static void ProcessGameTick(lua_State*, const char*);
		static void ProcessNewState(lua_State*);
		static void ProcessCloseState(lua_State*);
		static void __cdecl HookRequire(lua_State*);
		
		struct YieldCheck {
			bool isYield;
			int nargs;
		} yieldCheck;

		InGameHandling& inGame;
		PaydayLua();
		
	public:
		static PaydayLua& Get();

		int luaL_error(lua_State*, const char* fmt, ...) override;
		int luaL_loadbuffer(lua_State*, const char*, size_t,
							const char*) override;
		int luaL_loadfile(lua_State*, const char*) override;
		int luaL_loadstring(lua_State*, const char*) override;
		int luaL_newmetatable(lua_State*, const char*);
		void luaL_openlib(	lua_State*, const char*,
							const luaL_Reg*, int) override;
		int luaL_ref(lua_State*, int) override;
		void luaL_unref(lua_State*, int, int) override;
		void lua_call(lua_State*, int, int) override;
		int lua_checkstack(lua_State*, int) override;
		void lua_close(lua_State*) override;
		void lua_concat(lua_State*, int) override;
		void lua_createtable(lua_State*, int, int) override;
		int lua_dump(lua_State*, lua_Writer, void*) override;
		int lua_equal(lua_State *, int, int) override;
		int lua_error(lua_State*) override;
		int lua_gc(lua_State*, int, int) override;
		int lua_getinfo(lua_State*, const char*, lua_Debug*) override;
		void lua_getfield(lua_State*, int, const char*) override;
		lua_Hook lua_gethook(lua_State*) override;
		int lua_gethookcount(lua_State*) override;
		int lua_gethookmask(lua_State*) override;
		int lua_getmetatable(lua_State *, int) override;
		int lua_getstack(lua_State*, int, lua_Debug*) override;
		void lua_gettable(lua_State*, int) override;
		int lua_gettop(lua_State*) override;
		void lua_insert(lua_State*, int) override;
		int lua_isnumber(lua_State*, int) override;
		int lua_lessthan(lua_State*, int, int) override;
		int lua_load(lua_State*, lua_Reader, void*, const char*) override;
		lua_State* lua_newthread(lua_State*) override;
		void* lua_newuserdata(lua_State*, size_t) override;
		int lua_next(lua_State*, int) override;
		size_t lua_objlen(lua_State*, int) override;
		int lua_pcall(lua_State*, int, int, int) override;
		void lua_pushboolean(lua_State*, int) override;
		void lua_pushcclosure(lua_State*, lua_CPPFunction, int) override;
		const char* lua_pushfstring(lua_State*, const char*, ...) override;
		void lua_pushinteger(lua_State*, lua_Integer) override;
		void lua_pushlightuserdata(lua_State*, void*) override;
		void lua_pushlstring(lua_State*, const char*, size_t) override;
		void lua_pushnil(lua_State*) override;
		void lua_pushnumber(lua_State*, lua_Number) override;
		void lua_pushstring(lua_State*, const char*) override;
		int lua_pushthread(lua_State*) override;
		void lua_pushvalue(lua_State*, int) override;
		const char* lua_pushvfstring(	lua_State*, const char*,
										va_list) override;
		int lua_rawequal(lua_State*, int, int) override;
		void lua_rawget(lua_State*, int) override;
		void lua_rawgeti(lua_State*, int, int) override;
		void lua_rawset(lua_State*, int) override;
		void lua_rawseti(lua_State*, int, int) override;
		void lua_remove(lua_State*, int) override;
		void lua_replace(lua_State*, int) override;
		int lua_resume(lua_State*, int) override;
		void lua_setfield(lua_State*, int, const char*) override;
		int lua_sethook(lua_State*, lua_Hook, int, int) override;
		int lua_setmetatable(lua_State*, int) override;
		void lua_settable(lua_State*, int) override;
		void lua_settop(lua_State*, int) override;
		int lua_toboolean(lua_State*, int) override;
		lua_CFunction lua_tocfunction(lua_State*, int) override;
		int lua_tointeger(lua_State*, int) override;
		const char* lua_tolstring(lua_State*, int, size_t*) override;
		lua_Number lua_tonumber(lua_State*, int) override;
		lua_State* lua_tothread(lua_State*, int) override;
		void* lua_touserdata(lua_State*, int) override;
		int lua_type(lua_State*, int) override;
		const char* lua_typename(lua_State*, int) override;
		void lua_xmove(lua_State*, lua_State*, int) override;
		int lua_yield(lua_State*, int) override;
		
	};
}
