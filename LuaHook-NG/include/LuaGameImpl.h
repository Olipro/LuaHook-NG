#pragma once
#include "LuaInterface.h"
#include "LuaGame.h"

namespace Olipro {
	class LuaGameImpl {
		static std::vector<LuaGameImpl*> instances;
		static std::shared_mutex mutex;

		std::function<void(lua_State*, LuaInterface&)> gameTickClbk;
		std::function<void(lua_State*, LuaInterface&)> newStateClbk;
		std::function<void(lua_State*, LuaInterface&)> closeStateClbk;
		std::function<void(lua_State*, LuaInterface&,
			const std::string&)> requireClbk;

		static void OnGameTick(lua_State*, LuaInterface&);
		static void OnNewState(lua_State*, LuaInterface&);
		static void OnCloseState(lua_State*, LuaInterface&);
		static void OnRequire(lua_State*, LuaInterface&, const std::string&);
		friend class LuaInterface;

	public:
		LuaGameImpl(decltype(gameTickClbk), decltype(newStateClbk),
			decltype(closeStateClbk), decltype(requireClbk));
		~LuaGameImpl();
	};
}
