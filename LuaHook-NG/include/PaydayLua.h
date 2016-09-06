#pragma once
#include "LuaInterface.h"

namespace Olipro {

	class PaydayLua final : public LuaInterface
	{
		static LPVOID __fastcall HookDoGameTick(lua_State**, LPVOID,
												LPVOID, LPCSTR);
		static void __fastcall HookLuaLNewState(lua_State**, int, 
												int, int, int);
		static void __cdecl HookLuaSetField(const char*);

		struct InGameFunctionSignatures {
			void(__fastcall *lua_close)(lua_State*);
			void(__fastcall *lua_createtable)(lua_State*, int, int);
			void(__fastcall *lua_getfield)(lua_State*, int, const char*);
			void(__fastcall *lua_gettable)(lua_State*, int);
			void*(__fastcall *lua_newuserdata)(lua_State*, size_t);
			size_t(__fastcall *lua_objlen)(lua_State*, int);
			int(__fastcall *lua_pcall)(lua_State*, int, int, int);
			void(__fastcall *lua_pushcclosure)(lua_State*, lua_CFunction, int);
			void(__fastcall *lua_pushlstring)(lua_State*, const char*, size_t);
			const TValue*(__fastcall *luaH_getnum)(Table*, int);
			void(__fastcall *lua_rawgeti)(lua_State*, int, int);
			void(__fastcall *lua_rawset)(lua_State*, int);
			void(__fastcall *lua_rawseti)(lua_State*, int, int);
			void(__fastcall *lua_setfield)(lua_State*, int, const char*);
			void(__fastcall *lua_settop)(lua_State*, int);
			const char*(__fastcall *lua_tolstring)(lua_State*, int, size_t*);
			void(__fastcall *luaC_fullgc)(lua_State*);
			void(__fastcall *luaC_step)(lua_State*);
			void(__fastcall *luaD_call)(lua_State*, void*, int);
			lua_State*(__fastcall *luaE_newthread)(lua_State*);
			void(__fastcall *luaG_errormsg)(lua_State*);
			int(__fastcall *luaL_loadbuffer)(lua_State*, const char*,
				size_t, const char*);
			void(__fastcall *luaL_newstate)(lua_State**, int, int, int, int);
			int(__fastcall *luaL_ref)(lua_State*, int);
			void(__fastcall *luaL_unref)(lua_State*, int, int);
			const char*(__fastcall *luaO_pushvfstring)(	lua_State*,
														const char*, va_list);
			void(__fastcall *luaV_settable)(lua_State*, const TValue*,
				TValue*, StkId);
			decltype(HookDoGameTick)* tDoGameTick;
			decltype(luaL_newstate) tLuaL_newstate;
			decltype(lua_setfield) tLua_setfield;
		} static inGame;
		PaydayLua();
		
	public:
		static PaydayLua& Get();

		int luaL_loadfile(lua_State*, const char*) override;
		int luaL_loadstring(lua_State*, const char*) override;
		int luaL_ref(lua_State*, int) override;
		void luaL_unref(lua_State*, int, int) override;
		void lua_call(lua_State*, int, int) override;
		int lua_checkstack(lua_State *, int) override;
		void lua_close(lua_State*) override;
		void lua_concat(lua_State *, int) override;
		void lua_createtable(lua_State *, int, int) override;
		int lua_dump(lua_State*, lua_Writer, void*) override;
		int lua_equal(lua_State *, int, int) override;
		int lua_error(lua_State*) override;
		int lua_getinfo(lua_State*, const char*, lua_Debug*) override;
		void lua_getfield(lua_State*, int, const char*) override;
		int lua_getmetatable(lua_State *, int) override;
		void lua_gettable(lua_State*, int) override;
		int lua_gettop(lua_State*) override;
		void lua_insert(lua_State*, int) override;
		int lua_isnumber(lua_State*, int) override;
		int lua_lessthan(lua_State*, int, int) override;
		lua_State* lua_newthread(lua_State*) override;
		void* lua_newuserdata(lua_State*, size_t) override;
		int lua_next(lua_State*, int) override;
		size_t lua_objlen(lua_State*, int) override;
		int lua_pcall(lua_State*, int, int, int) override;
		void lua_pushboolean(lua_State*, int) override;
		void lua_pushcclosure(lua_State*, lua_CFunction, int) override;
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
		int lua_setmetatable(lua_State*, int) override;
		void lua_settable(lua_State*, int) override;
		void lua_settop(lua_State*, int) override;
		int lua_toboolean(lua_State*, int) override;
		void* lua_tocfunction(lua_State*, int) override;
		int lua_tointeger(lua_State*, int) override;
		const char* lua_tolstring(lua_State*, int, size_t*) override;
		lua_Number lua_tonumber(lua_State*, int) override;
		lua_State* lua_tothread(lua_State*, int) override;
		void* lua_touserdata(lua_State*, int) override;
		int lua_type(lua_State*, int) override;
		const char* lua_typename(lua_State*, int) override;
		void lua_xmove(lua_State*, lua_State*, int) override;
		int lua_yield(lua_State*, int) override;
		
		/*
		int lua_gc(lua_State*, int, int) override;
		
		int lua_gethookcount(lua_State*) override;
		int lua_gethookmask(lua_State*) override;
		
		const char* lua_getlocal(lua_State*, lua_Debug *, int) override;
		
		int lua_getstack(lua_State*, int, lua_Debug*) override;
		
		
		const char* lua_getupvalue(lua_State*, int, int) override;
		
		
		
		int lua_load(lua_State*, lua_Reader, void*, const char*, int) override;
		lua_State* lua_newstate(lua_Alloc*, void*) override;

		
		
		const char* lua_setlocal(lua_State*, lua_Debug*, int) override;
		int lua_sethook(lua_State*, lua_Debug*, int, int) override;
		
		
		const char* lua_setupvalue(lua_State*, int, int) override;
		
		
		
		const void* lua_topointer(lua_State*, int) override;
		
		int luaG_errormsg(lua_State*) override;
		void luaL_addlstring(luaL_Buffer*, const char*, size_t) override;
		int luaL_argerror(lua_State*, int, const char*) override;
		void luaL_buffinit(lua_State*, luaL_Buffer*) override;
		int luaL_checkinteger(lua_State*, int) override;
		const char* luaL_checklstring(lua_State*, int, size_t*) override;
		float luaL_checknumber(lua_State*, int) override;
		int luaL_checkoption(lua_State*, int, const char*, const char *const[]) override;
		void luaL_checkstack(lua_State*, int, const char*) override;
		void* luaL_checkudata(lua_State*, int, const char*) override;
		int luaL_error(lua_State*, const char* fmt, ...) override;
		int luaL_loadbuffer(lua_State*, const char*, size_t, const char*) override;
		
		int luaL_newmetatable(lua_State*, const char*) override;
		lua_State* luaL_newstate() override;
		void luaL_openlib(lua_State*, const char*, const luaL_Reg*, int) override;
		int luaL_optinteger(lua_State*, int, int) override;
		const char* luaL_optlstring(lua_State*, int, const char*, size_t*) override;
		float luaL_optnumber(lua_State*, int, float) override;
		void luaL_pushresult(luaL_Buffer*) override;
		
		int luaL_typerror(lua_State*, int, const char*) override;
		
		void luaL_where(lua_State*, int) override;

		const char* luaO_pushvfstring(lua_State*, const char*, va_list);
		int luaU_dump(lua_State*, const void*, void*, void*);

		void luaC_fullgc(lua_State*);
		void luaC_gcstep(lua_State*);
		int luaH_next(lua_State*, void*, void*);
		void luaV_settable(lua_State*, const TValue*, TValue*, TValue*);
		TValue* index2adr(lua_State*, int);
		void* luaH_new(lua_State*, int, int);
		lua_State* luaE_newthread(lua_State*);
		int luaD_pcall(lua_State*, void*, void*, ptrdiff_t, ptrdiff_t);*/
	};
}
