#include "stdafx.h"
#include "LuaGame.h"
#include "LuaGameImpl.h"

LuaGame::LuaGame(	std::function<void(lua_State*)> onTick,
					std::function<void(lua_State*)> onNew) : 
	impl{std::make_unique<LuaGameImpl>(onTick, onNew)}
{

}

void LuaHookNG()
{
}
