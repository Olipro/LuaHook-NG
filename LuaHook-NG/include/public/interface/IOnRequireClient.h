#pragma once

#include <string_view>

namespace LuaHookNG {
	struct lua_State;
	class IOnRequireClient {
	public:
		virtual void OnRequire(lua_State*, std::string_view) = 0;
	};
}  // namespace LuaHookNG
