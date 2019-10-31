#pragma once

#include "LuaAPI.h"
#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	struct lua_State;
	class LuaReimplementation {
		static inline int*(__cdecl* index2adr)(lua_State*, int);
		static inline int(__fastcall* lj_obj_equal)(int*, int*);
		static inline const void*(__fastcall* lj_obj_ptr)(int*);
		static inline void(__fastcall* lj_gc_check)(lua_State*);
		static inline const char*(__cdecl* lj_strfmt_pushvf)(lua_State*,
															 const char*,
															 va_list);
		static inline int(
		  __cdecl* lj_bcwrite)(lua_State*, void*, lua_Writer, void*, int);
		static inline int(__cdecl* luaL_loadfilex)(lua_State*,
												   const char*,
												   const char*);

	public:
		static void Init(IRuntimeCodeHook& codeHook);
		static int lua_iscfunction(lua_State* L, int n);
		static int lua_rawequal(lua_State* L, int a, int b);
		static lua_CFunction lua_tocfunction(lua_State* L, int idx);
		static void* lua_touserdata(lua_State* L, int idx);
		static lua_State* lua_tothread(lua_State* L, int idx);
		static const void* lua_topointer(lua_State* L, int idx);
		static const char* lua_pushvfstring(lua_State* L,
											const char* fmt,
											va_list args);
		static int lua_dump(lua_State* L, lua_Writer writer, void* data);
		static int lua_status(lua_State* L);
		static int luaL_loadfile(lua_State* L, const char* filename);
		static lua_State* luaL_newstate();
		static int NOTIMPL_lua_cpcall(lua_State*, lua_CFunction, void*);
		static int NOTIMPL_lua_yield(lua_State*, int);
	};
}  // namespace LuaHookNG
