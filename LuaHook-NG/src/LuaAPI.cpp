#include <cstdarg>

#include "detail/LuaAPIImpl.h"

namespace LuaHookNG {
	static auto& p = LuaFuncPtrs::inst;

	lua_State* lua_newstate(lua_Alloc a, void* b) {
		return p->lua_newstate_(a, b);
	}

	void lua_close(lua_State* L) {
		p->lua_close_(L);
	}

	lua_State* lua_newthread(lua_State* L) {
		return p->lua_newthread_(L);
	}

	int lua_gettop(lua_State* L) {
		return p->lua_gettop_(L);
	}

	void lua_settop(lua_State* L, int x) {
		p->lua_settop_(L, x);
	}

	void lua_pushvalue(lua_State* L, int x) {
		p->lua_pushvalue_(L, x);
	}

	void lua_remove(lua_State* L, int x) {
		p->lua_remove_(L, x);
	}

	void lua_insert(lua_State* L, int x) {
		p->lua_insert_(L, x);
	}

	void lua_replace(lua_State* L, int x) {
		p->lua_replace_(L, x);
	}

	int lua_checkstack(lua_State* L, int x) {
		return p->lua_checkstack_(L, x);
	}

	void lua_xmove(lua_State* from, lua_State* to, int n) {
		p->lua_xmove_(from, to, n);
	}

	int lua_isnumber(lua_State* L, int n) {
		return p->lua_isnumber_(L, n);
	}

	int lua_isstring(lua_State* L, int n) {
		return p->lua_isstring_(L, n);
	}

	int lua_iscfunction(lua_State* L, int n) {
		return p->lua_iscfunction_(L, n);
	}

	int lua_isuserdata(lua_State* L, int n) {
		return p->lua_isuserdata_(L, n);
	}

	int lua_type(lua_State* L, int n) {
		return p->lua_type_(L, n);
	}

	const char* lua_typename(lua_State* L, int n) {
		return p->lua_typename_(L, n);
	}

	int lua_equal(lua_State* L, int n, int x) {
		return p->lua_equal_(L, n, x);
	}

	int lua_rawequal(lua_State* L, int n, int x) {
		return p->lua_rawequal_(L, n, x);
	}

	int lua_lessthan(lua_State* L, int n, int x) {
		return p->lua_lessthan_(L, n, x);
	}

	lua_Number lua_tonumber(lua_State* L, int n) {
		return p->lua_tonumber_(L, n);
	}

	lua_Integer lua_tointeger(lua_State* L, int n) {
		return p->lua_tointeger_(L, n);
	}

	int lua_toboolean(lua_State* L, int n) {
		return p->lua_toboolean_(L, n);
	}

	const char* lua_tolstring(lua_State* L, int n, size_t* len) {
		return p->lua_tolstring_(L, n, len);
	}

	size_t lua_objlen(lua_State* L, int n) {
		return p->lua_objlen_(L, n);
	}

	lua_CFunction lua_tocfunction(lua_State* L, int n) {
		return p->lua_tocfunction_(L, n);
	}

	void* lua_touserdata(lua_State* L, int n) {
		return p->lua_touserdata_(L, n);
	}

	lua_State* lua_tothread(lua_State* L, int n) {
		return p->lua_tothread_(L, n);
	}

	const void* lua_topointer(lua_State* L, int n) {
		return p->lua_topointer_(L, n);
	}

	void lua_pushnil(lua_State* L) {
		p->lua_pushnil_(L);
	}

	void lua_pushnumber(lua_State* L, lua_Number n) {
		p->lua_pushnumber_(L, n);
	}

	void lua_pushinteger(lua_State* L, lua_Integer n) {
		p->lua_pushinteger_(L, n);
	}

	void lua_pushlstring(lua_State* L, const char* c, size_t sz) {
		p->lua_pushlstring_(L, c, sz);
	}

	void lua_pushstring(lua_State* L, const char* c) {
		p->lua_pushstring_(L, c);
	}

	const char* lua_pushvfstring(lua_State* L, const char* fmt, va_list v) {
		return p->lua_pushvfstring_(L, fmt, v);
	}

	const char* lua_pushfstring(lua_State* L, const char* fmt, ...) {
		va_list v;
		va_start(v, fmt);
		auto ret = p->lua_pushvfstring_(L, fmt, v);
		va_end(v);
		return ret;
	}

	void lua_pushcclosure(lua_State* L, lua_CFunction f, int n) {
		p->lua_pushcclosure_(L, f, n);
	}

	void lua_pushboolean(lua_State* L, int b) {
		p->lua_pushboolean_(L, b);
	}

	void lua_pushlightuserdata(lua_State* L, void* ud) {
		p->lua_pushlightuserdata_(L, ud);
	}

	int lua_pushthread(lua_State* L) {
		return p->lua_pushthread_(L);
	}

	void lua_gettable(lua_State* L, int n) {
		p->lua_gettable_(L, n);
	}

	void lua_getfield(lua_State* L, int n, const char* c) {
		p->lua_getfield_(L, n, c);
	}

	void lua_rawget(lua_State* L, int n) {
		p->lua_rawget_(L, n);
	}

	void lua_rawgeti(lua_State* L, int idx, int n) {
		p->lua_rawgeti_(L, idx, n);
	}

	void lua_createtable(lua_State* L, int narr, int nrec) {
		p->lua_createtable_(L, narr, nrec);
	}

	void* lua_newuserdata(lua_State* L, size_t sz) {
		return p->lua_newuserdata_(L, sz);
	}

	int lua_getmetatable(lua_State* L, int n) {
		return p->lua_getmetatable_(L, n);
	}

	void lua_getfenv(lua_State* L, int n) {
		p->lua_getfenv_(L, n);
	}

	void lua_settable(lua_State* L, int n) {
		p->lua_settable_(L, n);
	}

	void lua_setfield(lua_State* L, int n, const char* c) {
		p->lua_setfield_(L, n, c);
	}

	void lua_rawset(lua_State* L, int n) {
		p->lua_rawset_(L, n);
	}

	void lua_rawseti(lua_State* L, int idx, int n) {
		p->lua_rawseti_(L, idx, n);
	}

	void lua_setmetatable(lua_State* L, int n) {
		p->lua_setmetatable_(L, n);
	}

	void lua_setfenv(lua_State* L, int n) {
		p->lua_setfenv_(L, n);
	}

	void lua_call(lua_State* L, int nargs, int nresults) {
		p->lua_call_(L, nargs, nresults);
	}

	int lua_pcall(lua_State* L, int nargs, int nresults, int errfunc) {
		return p->lua_pcall_(L, nargs, nresults, errfunc);
	}

	int lua_cpcall(lua_State* L, lua_CFunction func, void* ud) {
		return p->lua_cpcall_(L, func, ud);
	}

	int lua_load(lua_State* L, lua_Reader r, void* dt, const char* chunkname) {
		return p->lua_load_(L, r, dt, chunkname);
	}

	int lua_dump(lua_State* L, lua_Writer w, void* data) {
		return p->lua_dump_(L, w, data);
	}

	int lua_yield(lua_State* L, int nresults) {
		return p->lua_yield_(L, nresults);
	}

	int lua_resume(lua_State* L, int narg) {
		return p->lua_resume_(L, narg);
	}

	int lua_status(lua_State* L) {
		return p->lua_status_(L);
	}

	int lua_next(lua_State* L, int n) {
		return p->lua_next_(L, n);
	}

	void lua_concat(lua_State* L, int n) {
		p->lua_concat_(L, n);
	}

	int luaL_ref(lua_State* L, int t) {
		return p->luaL_ref_(L, t);
	}

	void luaL_unref(lua_State* L, int t, int ref) {
		p->luaL_unref_(L, t, ref);
	}

	int luaL_loadfile(lua_State* L, const char* filename) {
		return p->luaL_loadfile_(L, filename);
	}

	int luaL_loadbuffer(lua_State* L,
						const char* buf,
						size_t sz,
						const char* name) {
		return p->luaL_loadbuffer_(L, buf, sz, name);
	}

	int luaL_loadstring(lua_State* L, const char* s) {
		return p->luaL_loadstring_(L, s);
	}

	int luaL_newmetatable(lua_State* L, const char* s) {
		return p->luaL_newmetatable_(L, s);
	}

	lua_State* luaL_newstate() {
		return p->luaL_newstate_();
	}
}  // namespace LuaHookNG