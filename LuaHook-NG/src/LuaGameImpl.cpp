#include "stdafx.h"
#include "LuaGameImpl.h"

std::vector<LuaGameImpl*> LuaGameImpl::instances;
std::shared_mutex LuaGameImpl::mutex;

LuaGameImpl::LuaGameImpl(	decltype(gameTickClbk) tick,
							decltype(newStateClbk) newState) :
	gameTickClbk{ tick }, newStateClbk{ newState }
{
	std::unique_lock<decltype(mutex)> lock{ mutex };
	instances.emplace_back(this);
}

LuaGameImpl::~LuaGameImpl()
{
	std::unique_lock<decltype(mutex)> lock{ mutex };
	std::remove(instances.begin(), instances.end(), this);
}

void LuaGameImpl::OnGameTick(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->gameTickClbk)
			inst->gameTickClbk(L), lua.lua_settop(L, 0);
}

void LuaGameImpl::OnNewState(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->newStateClbk)
			inst->newStateClbk(L), lua.lua_settop(L, 0);
}
