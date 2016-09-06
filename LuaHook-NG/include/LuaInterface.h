#pragma once

namespace Olipro {

	class LuaInterface {
	protected:
		void OnGameTick(lua_State* L);
		void OnNewState(lua_State* L);
	public:
		virtual int luaL_loadfile(lua_State*, const char*) = 0;
		virtual int luaL_loadstring(lua_State*, const char*) = 0;
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
		virtual void lua_getfield(lua_State*, int, const char*) = 0;
		virtual int lua_getinfo(lua_State*, const char*, lua_Debug*) = 0;
		virtual int lua_getmetatable(lua_State *, int) = 0;
		virtual void lua_gettable(lua_State*, int) = 0;
		virtual int lua_gettop(lua_State*) = 0;
		virtual void lua_insert(lua_State*, int) = 0;
		virtual int lua_isnumber(lua_State*, int) = 0;
		virtual int lua_lessthan(lua_State*, int, int) = 0;
		virtual void* lua_newuserdata(lua_State*, size_t) = 0;
		virtual lua_State* lua_newthread(lua_State*) = 0;
		virtual int lua_next(lua_State*, int) = 0;
		virtual size_t lua_objlen(lua_State*, int) = 0;
		virtual int lua_pcall(lua_State*, int, int, int) = 0;
		virtual void lua_pushboolean(lua_State*, int) = 0;
		virtual void lua_pushcclosure(lua_State*, lua_CFunction, int) = 0;
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
		virtual int lua_setmetatable(lua_State*, int) = 0;
		virtual void lua_settable(lua_State*, int) = 0;
		virtual void lua_settop(lua_State*, int) = 0;
		virtual int lua_tointeger(lua_State*, int) = 0;
		virtual const char* lua_tolstring(lua_State*, int, size_t*) = 0;
		virtual int lua_toboolean(lua_State*, int) = 0;
		virtual void* lua_tocfunction(lua_State*, int) = 0;
		virtual lua_Number lua_tonumber(lua_State*, int) = 0;
		virtual lua_State* lua_tothread(lua_State*, int) = 0;
		virtual void* lua_touserdata(lua_State*, int) = 0;
		virtual int lua_type(lua_State*, int) = 0;
		virtual const char* lua_typename(lua_State*, int) = 0;
		virtual void lua_xmove(lua_State*, lua_State*, int) = 0;
		virtual int lua_yield(lua_State*, int) = 0;

		virtual ~LuaInterface() = default;
		/*
		virtual int lua_gc(lua_State*, int, int) = 0;
		virtual int lua_gethookcount(lua_State*) = 0;
		virtual int lua_gethookmask(lua_State*) = 0;
		
		virtual const char* lua_getlocal(lua_State*, lua_Debug *ar, int n) = 0;
		
		virtual int lua_getstack(lua_State*, int, lua_Debug*) = 0;
		virtual const char* lua_getupvalue(lua_State*, int, int) = 0;
		
		
		
		virtual int lua_load(lua_State*, lua_Reader, void*, const char*, int) = 0;
		virtual lua_State* lua_newstate(lua_Alloc*, void*) = 0;
		
		
		virtual const char* lua_setlocal(lua_State*, lua_Debug*, int) = 0;
		virtual int lua_sethook(lua_State*, lua_Debug*, int, int) = 0;
		virtual const char* lua_setupvalue(lua_State*, int, int) = 0;

		virtual const void* lua_topointer(lua_State*, int) = 0;
		
		
		
		virtual int luaG_errormsg(lua_State*) = 0;
		virtual void luaL_addlstring(luaL_Buffer*, const char*, size_t) = 0;
		virtual int luaL_argerror(lua_State*, int, const char*) = 0;
		virtual void luaL_buffinit(lua_State*, luaL_Buffer*) = 0;
		virtual int luaL_checkinteger(lua_State*, int) = 0;
		virtual const char* luaL_checklstring(lua_State*, int, size_t*) = 0;
		virtual float luaL_checknumber(lua_State*, int) = 0;
		virtual int luaL_checkoption(lua_State*, int, const char*, const char *const[]) = 0;
		virtual void luaL_checkstack(lua_State*, int, const char*) = 0;
		virtual void* luaL_checkudata(lua_State*, int, const char*) = 0;
		virtual int luaL_error(lua_State*, const char* fmt, ...) = 0;
		virtual int luaL_loadbuffer(lua_State*, const char*, size_t, const char*) = 0;
		
		virtual int luaL_newmetatable(lua_State*, const char*) = 0;
		virtual lua_State* luaL_newstate() = 0;
		virtual void luaL_openlib(lua_State*, const char*, const luaL_Reg*, int) = 0;
		virtual int luaL_optinteger(lua_State*, int, int) = 0;
		virtual const char* luaL_optlstring(lua_State*, int, const char*, size_t*) = 0;
		virtual float luaL_optnumber(lua_State*, int, float) = 0;
		virtual void luaL_pushresult(luaL_Buffer*) = 0;
		
		virtual int luaL_typerror(lua_State*, int, const char*) = 0;
		
		virtual void luaL_where(lua_State*, int) = 0;*/

		/*	virtual const char* luaO_pushvfstring(lua_State*, const char*, va_list) = 0;
			virtual int luaU_dump(lua_State*, const void*, void*, void*) = 0;
			virtual void luaC_fullgc(lua_State*) = 0;
			virtual void luaC_gcstep(lua_State*) = 0;
			virtual int luaH_next(lua_State*, void*, void*) = 0;
			virtual void luaV_settable(lua_State*, const TValue*, TValue*, TValue*) = 0;
			virtual TValue* index2adr(lua_State*, int) = 0;
			virtual void* luaH_new(lua_State*, int, int) = 0;
			virtual lua_State* luaE_newthread(lua_State*) = 0;
			virtual int luaD_pcall(lua_State*, void*, void*, ptrdiff_t, ptrdiff_t) = 0;*/
	};

	enum class MandatoryLua {
		lua_call,
		lua_checkstack,
		lua_close,
		lua_concat,
		lua_createtable,
		lua_equal,
		lua_gc,
		lua_getfield,
		lua_getinfo,
		lua_getlocal,
		lua_getmetatable,
		lua_getstack,
		lua_gettable,
		lua_gettop,
		lua_getupvalue,
		lua_insert,
		lua_isnumber,
		lua_lessthan,
		lua_load,
		lua_newstate,
		lua_newthread,
		lua_newuserdata,
		lua_next,
		lua_objlen,
		lua_pcall,
		lua_pushboolean,
		lua_pushcclosure,
		lua_pushfstring,
		lua_pushinteger,
		lua_pushlstring,
		lua_pushnil,
		lua_pushnumber,
		lua_pushstring,
		lua_pushthread,
		lua_pushvalue,
		lua_pushvfstring,
		lua_rawequal,
		lua_rawget,
		lua_rawgeti,
		lua_rawset,
		lua_rawseti,
		lua_remove,
		lua_replace,
		lua_resume,
		lua_setfield,
		lua_sethook,
		lua_setlocal,
		lua_setmetatable,
		lua_settable,
		lua_settop,
		lua_setupvalue,
		lua_toboolean,
		lua_tointeger,
		lua_tolstring,
		lua_tonumber,
		lua_topointer,
		lua_tothread,
		lua_touserdata,
		lua_type,
		lua_typename,
		lua_xmove,
		lua_yield,
		luaG_errormsg,
		luaL_argerror,
		luaL_checkinteger,
		luaL_checklstring,
		luaL_checknumber,
		luaL_checkoption,
		luaL_checkudata,
		luaL_error,
		luaL_loadbuffer,
		luaL_loadfile,
		luaL_loadstring,
		luaL_newstate,
		luaL_openlib,
		luaL_optinteger,
		luaL_optlstring,
		luaL_optnumber,
		luaL_ref,
		luaL_typerror,
		luaL_unref,
		luaL_where,
		luaO_pushvfstring,
		luaU_dump,
		luaV_execute,
		index2adr,
#ifdef TEC_BUILD
		SetupSteamFunctions,
#endif
		END,
	};
}
