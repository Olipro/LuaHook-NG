#pragma once
#include "LuaInterface.h"
#include "LuaGame.h"

using namespace Olipro;

class LuaGameImpl {
	static std::vector<LuaGameImpl*> instances;
	static std::shared_mutex mutex;
	std::function<void(lua_State*)> gameTickClbk;
	std::function<void(lua_State*)> newStateClbk;

	static void OnGameTick(lua_State*, LuaInterface& lua);
	static void OnNewState(lua_State*, LuaInterface& lua);
	friend class LuaInterface;

public:
	LuaGameImpl(decltype(gameTickClbk), decltype(newStateClbk));
	~LuaGameImpl();
};
