#pragma once

namespace LuaHookNG {
	struct lua_State;
	class ICloseStateClient {
	public:
		virtual void OnCloseState(lua_State*) = 0;
	};
}  // namespace LuaHookNG
