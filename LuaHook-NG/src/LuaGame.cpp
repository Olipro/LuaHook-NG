#include "stdafx.h"
#include "LuaGame.h"
#include "LuaGameImpl.h"

using namespace Olipro;

LuaGame::LuaGame(	std::function<void(lua_State*, LuaInterface&)> onTick,
					std::function<void(lua_State*, LuaInterface&)> onNew,
					std::function<void(lua_State*, LuaInterface&)> onClose,
					std::function<void(lua_State*, LuaInterface&,
						const std::string&)> onRequire) :
	impl{std::make_unique<LuaGameImpl>(onTick, onNew, onClose, onRequire)}
{

}

LuaGame::~LuaGame()
{
}

LuaGame& LuaGame::operator<<(const std::string& s)
{
	return std::cout << s, *this;
}
