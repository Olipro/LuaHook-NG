#pragma once
#include "Lua.h"

namespace Olipro {
	class InGameHandling {
	public:
		struct InGameFunctionSignatures;
	private:
		void(&ProcessGameTick)(lua_State*, const char* op);
		void(&ProcessNewState)(lua_State*);
		void(&ProcessCloseState)(lua_State*);

		InGameHandling(decltype(ProcessGameTick), decltype(ProcessNewState),
			decltype(ProcessCloseState));
		InGameFunctionSignatures LoadSignatures();

		static void* __fastcall HookDoGameTick(	lua_State**, int, void*,
												const char*);
		static decltype(HookDoGameTick)* tGameTick;
		static void __fastcall HookNewState(lua_State**, int, int, int, int);
		static decltype(HookNewState)* tNewState;
		static void __fastcall HookLuaClose(lua_State*);
		static decltype(HookLuaClose)* tCloseState;
		static void __fastcall HookLuaSetField(lua_State*, int, const char*);
		static decltype(HookLuaSetField)* tlua_setfield;

		static void __fastcall OnGameTick(lua_State*, const char*);
		static void __fastcall OnLuaSetField(lua_State*, int, const char*);
		static InGameHandling* self;

	public:
		static InGameHandling& GetInstance(decltype(ProcessGameTick),
			decltype(ProcessNewState), decltype(ProcessCloseState));
		static InGameHandling& GetInstance();

		struct InGameFunctionSignatures {
			int(__fastcall &lua_checkstack)(lua_State*, int);
			void(__fastcall &lua_close)(lua_State*);
			void(__fastcall &lua_createtable)(lua_State*, int, int);
			void(__fastcall &lua_getfield)(lua_State*, int, const char*);
			int(__fastcall &lua_getinfo)(lua_State*, const char*, lua_Debug*);
			void(__fastcall &lua_gettable)(lua_State*, int);
			void*(__fastcall &lua_newuserdata)(lua_State*, size_t);
			size_t(__fastcall &lua_objlen)(lua_State*, int);
			int(__fastcall &lua_pcall)(lua_State*, int, int, int);
			void(__fastcall &lua_pushcclosure)(lua_State*, lua_CFunction, int);
			void(__fastcall &lua_pushlstring)(lua_State*, const char*, size_t);
			void(__fastcall &lua_rawgeti)(lua_State*, int, int);
			void(__fastcall &lua_rawset)(lua_State*, int);
			void(__fastcall &lua_rawseti)(lua_State*, int, int);
			int(__fastcall &lua_resume)(lua_State*, int);
			void(__fastcall &lua_setfield)(lua_State*, int, const char*);
			int(__fastcall &lua_setmetatable)(lua_State*, int);
			void(__fastcall &lua_settop)(lua_State*, int);
			const char*(__fastcall &lua_tolstring)(lua_State*, int, size_t*);
			void(__fastcall &luaC_fullgc)(lua_State*);
			void(__fastcall &luaC_step)(lua_State*);
			void(__fastcall &luaD_call)(lua_State*, void*, int);
			int(__fastcall &luaD_protectedparser)(lua_State*, ZIO*,
				const char*);
			lua_State*(__fastcall &luaE_newthread)(lua_State*);
			void(__fastcall &luaG_errormsg)(lua_State*);
			const TValue*(__fastcall &luaH_getnum)(Table*, int);
			int(__cdecl &luaL_error)(/*lua_State*, const char*, ... */);
			const char*(__fastcall &luaL_findtable)(lua_State*, int,
				const char*, int);
			int(__fastcall &luaL_loadbuffer)(lua_State*, const char*,
				size_t, const char*);
			int(__fastcall &luaL_loadfile)(lua_State*, const char*);
			void(__fastcall &luaL_newstate)(lua_State**, int, int, int, int);
			const char*(__fastcall &luaO_pushvfstring)(lua_State*,
				const char*, va_list);
			void(__fastcall &luaV_settable)(lua_State*, const TValue*,
				TValue*, StkId);
			/*LPVOID(__thiscall &tDoGameTick)(lua_State**, LPVOID, LPCSTR);
			decltype(lua_close) tLuaClose;
			decltype(luaL_newstate) tLuaL_newstate;
			decltype(lua_setfield) tLua_setfield;*/
		} const functions;
	};
}
