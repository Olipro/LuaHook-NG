#pragma once
#include "ConsoleManager.h"

namespace LuaHookNG {
	class DeveloperTools {
		static DeveloperTools self;
		ConsoleManager console;

		DeveloperTools();
	};
}  // namespace LuaHookNG
