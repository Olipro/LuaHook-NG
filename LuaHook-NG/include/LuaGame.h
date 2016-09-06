#pragma once

#ifndef LUAHOOKNG_EXPORTS
#pragma comment (lib, "LuaHook-NG.lib")
#define EXTERNAL __declspec(dllimport)
#include <functional>
#include <memory>
typedef void lua_State;
#endif

class LuaGameImpl;

class EXTERNAL LuaGame final {
#pragma warning(suppress: 4251)
	std::unique_ptr<LuaGameImpl> impl;

public:
	LuaGame(std::function<void(lua_State*)>, std::function<void(lua_State*)>);
};

extern "C" EXTERNAL void LuaHookNG(); //dummy function for forcing linkage.
