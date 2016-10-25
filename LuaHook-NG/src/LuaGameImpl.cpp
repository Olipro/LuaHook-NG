#include "stdafx.h"
#include "LuaGameImpl.h"

using namespace Olipro;

std::vector<LuaGameImpl*> LuaGameImpl::instances;
std::vector<std::remove_const_t<decltype(LuaGameImpl::gameTickClbk)>>
	LuaGameImpl::tickClbks;
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
	if (gameTickClbk)
		tickClbks.emplace_back(gameTickClbk);
}

LuaGameImpl::~LuaGameImpl()
{
	std::unique_lock<decltype(mutex)> lock{ mutex };
	instances.erase(std::find(instances.begin(), instances.end(), this));
	if (gameTickClbk)
		RebuildGameTickCallbacks();
}

void LuaGameImpl::RebuildGameTickCallbacks()
{
	tickClbks.clear();
	for (auto i : instances)
		tickClbks.emplace_back(i->gameTickClbk);
}

template <typename T>
void LuaGameImpl::CallWrapper(lua_State* L, LuaInterface& lua, T clbk)
{
	lua.lua_pushcclosure(L, [](lua_State* L, LuaInterface& lua) {
		auto&& clbk = *reinterpret_cast<T>(lua.lua_touserdata(L, 1));
		clbk(L, lua);
		return 0;
	}, 0);
	lua.lua_pushlightuserdata(L, const_cast<void*>(
		reinterpret_cast<const void*>(clbk)));
	lua.lua_call(L, 1, 0);
}

void LuaGameImpl::OnGameTick(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& clbk : tickClbks)
		CallWrapper(L, lua, &clbk);
}

void LuaGameImpl::OnNewState(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->newStateClbk)
			CallWrapper(L, lua, &inst->newStateClbk);
}

void LuaGameImpl::OnCloseState(lua_State* L, LuaInterface& lua)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->closeStateClbk)
			CallWrapper(L, lua, &inst->closeStateClbk);
}

void LuaGameImpl::OnRequire(lua_State*L, LuaInterface& lua,
	const std::string& name)
{
	std::shared_lock<decltype(mutex)> lock{ mutex };
	for (auto&& inst : instances)
		if (inst->requireClbk) {
			lua.lua_pushcclosure(L, [](lua_State* L, LuaInterface& lua) {
				auto&& clbk = *reinterpret_cast<
					decltype(LuaGameImpl::requireClbk)*>(
						lua.lua_touserdata(L, 1));
				auto&& file = *reinterpret_cast<decltype(&name)>(
					lua.lua_touserdata(L, 2));
				clbk(L, lua, file);
				return 0;
			}, 0);
			lua.lua_pushlightuserdata(L, const_cast<void*>(
				reinterpret_cast<const void*>(&inst->requireClbk)));
			lua.lua_pushlightuserdata(L, &const_cast<std::string&>(name));
			lua.lua_call(L, 2, 0);
		}
}
