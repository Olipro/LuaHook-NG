#include "stdafx.h"
#include "LuaGameImpl.h"

using namespace Olipro;

std::vector<LuaGameImpl*> LuaGameImpl::instances;
std::shared_mutex LuaGameImpl::mutex;

LuaGameImpl::LuaGameImpl(	decltype(gameTickClbk) tick,
							decltype(newStateClbk) newState,
							decltype(closeStateClbk) closeState,
							decltype(requireClbk) require) :
	gameTickClbk{ tick }, newStateClbk{ newState }, 
	closeStateClbk{ closeState }, requireClbk{ require }
{
	std::unique_lock<decltype(mutex)> lock{ mutex };
	instances.emplace_back(this);
}

LuaGameImpl::~LuaGameImpl()
{
	std::unique_lock<decltype(mutex)> lock{ mutex };
	instances.erase(std::remove(instances.begin(), instances.end(), this),
		instances.end());
}

void LuaGameImpl::OnGameTick(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->gameTickClbk)
			inst->gameTickClbk(L, lua);
}

void LuaGameImpl::OnNewState(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->newStateClbk)
			inst->newStateClbk(L, lua);
}

void LuaGameImpl::OnCloseState(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->closeStateClbk)
			inst->closeStateClbk(L, lua);
}

void LuaGameImpl::OnRequire(lua_State*L, LuaInterface& lua,
	const std::string& name)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->requireClbk)
			inst->requireClbk(L, lua, name);
}
