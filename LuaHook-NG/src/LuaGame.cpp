#include "stdafx.h"
#include "LuaGame.h"
#include "LuaGameImpl.h"

using namespace Olipro;

LuaGame::LuaGame(const CallbackArgs& clbkArgs) :
	impl{std::make_unique<LuaGameImpl>(clbkArgs.onTick, clbkArgs.onNew,
		clbkArgs.onClose, clbkArgs.onRequire)}
{

}

LuaGame::~LuaGame()
{
}

LuaGame& LuaGame::operator<<(const std::string& s)
{
	return std::cout << s, *this;
}
