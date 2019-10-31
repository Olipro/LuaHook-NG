#pragma once

namespace LuaHookNG {
	struct lua_State;
	class INewStateClient {
	public:
		virtual void OnNewState(lua_State*) = 0;
	};
}  // namespace LuaHookNG
