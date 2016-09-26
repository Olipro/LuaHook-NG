#pragma once
#include "LuaInterface.h"
#include <functional>
#include <memory>

#ifndef LUAHOOKNG_EXPORTS
#pragma comment (lib, "LuaHook-NG.lib")
#define EXTERNAL __declspec(dllimport)
#endif

namespace Olipro {
	class LuaGameImpl;

	class EXTERNAL LuaGame final {
#pragma warning(suppress: 4251)
		const std::unique_ptr<LuaGameImpl> impl;

	public:
		LuaGame(std::function<void(lua_State*, LuaInterface&)>,
			std::function<void(lua_State*, LuaInterface&)> = nullptr,
			std::function<void(lua_State*, LuaInterface&)> = nullptr,
			std::function<void(lua_State*, LuaInterface&,
				const std::string&)> = nullptr);
		~LuaGame();
		LuaGame& operator<<(const std::string&);
	};
}

extern "C" EXTERNAL void LuaHookNG(); //dummy function for forcing linkage.
