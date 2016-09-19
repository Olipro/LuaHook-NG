#pragma once
#include "LuaInterface.h"

#ifndef LUAHOOKNG_EXPORTS
#pragma comment (lib, "LuaHook-NG.lib")
#define EXTERNAL __declspec(dllimport)
#include <functional>
#include <memory>
#endif

namespace Olipro {
	class LuaGameImpl;

	class EXTERNAL LuaGame final {
#pragma warning(suppress: 4251)
		std::unique_ptr<LuaGameImpl> impl;

	public:
		LuaGame(std::function<void(lua_State*, LuaInterface&)>,
			std::function<void(lua_State*, LuaInterface&)>,
			std::function<void(lua_State*, LuaInterface&)>,
			std::function<void(lua_State*, LuaInterface&,
				const std::string&)>);
		~LuaGame();
		LuaGame& operator<<(const std::string&);
		LuaInterface& GetLua();
	};
}

extern "C" EXTERNAL void LuaHookNG(); //dummy function for forcing linkage.
