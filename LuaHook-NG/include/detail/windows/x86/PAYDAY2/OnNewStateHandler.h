#pragma once

#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	struct lua_State;
	class OnNewStateHandler {
		static void SetFieldHandler_ASM();
		static void __fastcall SetFieldHandler(lua_State* L,
											   int tbl,
											   const char* field);
		friend void __fastcall OnNewState(lua_State** L,
										  void* edx,
										  void* a,
										  void* b,
										  void* c);

	public:
		static void Init(IRuntimeCodeHook& cHook);
	};
}  // namespace LuaHookNG
