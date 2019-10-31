#pragma once
#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	struct lua_State;
	class OnTickHandler {
		static void OnGameTick_ASM();
		static void __cdecl SetOnGameTickTrampoline_ASM(void* addr);
		static void __fastcall OnGameTick(lua_State* L, const char* op);

	public:
		static void Init(IRuntimeCodeHook& codeHook);
	};
}  // namespace LuaHookNG
