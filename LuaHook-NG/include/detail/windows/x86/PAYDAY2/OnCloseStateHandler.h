#pragma once

#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	class OnCloseStateHandler {
	public:
		static void Init(IRuntimeCodeHook& codeHook);
	};
}  // namespace LuaHookNG
