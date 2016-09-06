#include "stdafx.h"
#include "LuaInterface.h"
#include "LuaGameImpl.h"

using namespace Olipro;

void LuaInterface::OnGameTick(lua_State* L)
{
	LuaGameImpl::OnGameTick(L, *this);
}

void LuaInterface::OnNewState(lua_State* L)
{
	LuaGameImpl::OnNewState(L, *this);
}
