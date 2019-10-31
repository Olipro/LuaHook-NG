#pragma once

namespace LuaHookNG {
	struct lua_State;
	class IOnTickClient {
	public:
		virtual void OnTick(lua_State*) = 0;
	};
}  // namespace LuaHookNG
