#include <catch.hpp>

#include "LuaHookNG.h"

using namespace LuaHookNG;
using namespace std::literals;

TEST_CASE("Lua State can have its stack manipulated properly") {
	auto L = luaL_newstate();
	REQUIRE(L != nullptr);
	CHECK(lua_gettop(L) == 0);
	lua_pushnumber(L, 3.141);
	CHECK(lua_gettop(L) == 1);
	CHECK(lua_tonumber(L, -1) == 3.141);
	CHECK(lua_isnumber(L, -1) == 1);
	lua_createtable(L, 0, 0);
	lua_pushstring(L, "hello");
	lua_setfield(L, -2, "aKey");
	lua_getfield(L, -1, "aKey");
	size_t len;
	auto str = lua_tolstring(L, -1, &len);
	CHECK(len == 5);
	CHECK("hello"sv == str);
	lua_settop(L, 0);
	luaL_loadstring(L, "return 1234");
	lua_call(L, 0, 1);
	CHECK(lua_gettop(L) == 1);
	CHECK(lua_tointeger(L, -1) == 1234);
	std::string_view buf{"return \"hi\""};
	luaL_loadbuffer(L, buf.data(), buf.length(), "test");
	lua_call(L, 0, 1);
	CHECK("hi"sv == lua_tolstring(L, -1, nullptr));
	lua_close(L);
}

TEST_CASE("Reimplemented functions for PAYDAY2 work properly") {
	auto L = luaL_newstate();
	lua_pushcclosure(
	  L,
	  [](auto) {
		  return 0;
	  },
	  0);
	CHECK(lua_iscfunction(L, -1) == 1);
	lua_pushstring(L, "x");
	CHECK(lua_iscfunction(L, -1) == 0);
	lua_pushinteger(L, 43);
	lua_pushinteger(L, 43);
	CHECK(lua_rawequal(L, -1, -2) == 1);
	CHECK(lua_rawequal(L, -1, -3) == 0);
	static bool directSet = false;
	lua_pushcclosure(
	  L,
	  [](auto) {
		  directSet = true;
		  return 2;
	  },
	  0);
	auto fn = lua_tocfunction(L, -1);
	CHECK(fn(L) == 2);
	CHECK(directSet == true);
	auto ud = lua_newuserdata(L, 4);
	CHECK(lua_touserdata(L, -1) == ud);
	lua_pushlightuserdata(L, &directSet);
	CHECK(lua_touserdata(L, -1) == &directSet);
	auto L2 = lua_newthread(L);
	CHECK(lua_tothread(L, -1) == L2);
	lua_close(L);
}
