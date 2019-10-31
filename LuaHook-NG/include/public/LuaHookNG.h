#pragma once

#include "LuaAPI.h"
#include "LuaAppLink.h"

namespace LuaHookNG {
	struct LuaHookVersion {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};
	extern "C" DLLSPEC void
	  LuaHook_NG();	 // Used internally to force linkage. Does nothing.
	extern "C" DLLSPEC LuaHookVersion Version();
}  // namespace LuaHookNG
