// TestLuaHookNG.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

using namespace Olipro;

struct TestCallbacks : public ::testing::Test
{
	std::promise<bool> wasInvoked;

	void Callback(lua_State* L, LuaInterface&)
	{
		EXPECT_NE(L, nullptr);
		wasInvoked.set_value(true);
	}

	auto GenerateLambda()
	{
		return [this](lua_State* L, LuaInterface& lua) { Callback(L, lua); };
	}

	template<typename T = std::chrono::seconds>
	void DoWaitTest(T timeout = 10s)
	{
		auto&& future = wasInvoked.get_future();
		EXPECT_NE(future.wait_for(timeout), std::future_status::timeout);
		EXPECT_TRUE(future.get());
	}
};

TEST_F(TestCallbacks, TestOnNewStateIsCalled) {
	LuaGame game{ nullptr, GenerateLambda() };
	DoWaitTest();
}

TEST_F(TestCallbacks, TestCloseStateIsCalled) {
	LuaGame game{ nullptr, nullptr, GenerateLambda() };
	DoWaitTest();
}

TEST_F(TestCallbacks, TestRequireIsCalled) {
	LuaGame game{ nullptr, nullptr, nullptr, [this](lua_State* L,
		LuaInterface& lua, const std::string& s) {
		Callback(L, lua);
		EXPECT_NE(s, "");
	} };
	DoWaitTest();
}

TEST_F(TestCallbacks, TestGameTickIsCalled) {
	LuaGame game{ GenerateLambda() };
	DoWaitTest();
}

TEST_F(TestCallbacks, TestGettingIntegerViaLuaFunction) {
	LuaGame game{ [this](lua_State* L, LuaInterface& lua) {
		const int expectedValue = 12345;
		lua.luaL_loadstring(L, "return ...");
		lua.lua_pushinteger(L, expectedValue);
		lua.lua_call(L, 1, 1);
		EXPECT_EQ(lua.lua_tointeger(L, -1), expectedValue);
		wasInvoked.set_value(true);
	}};
	DoWaitTest(1s);
}

TEST_F(TestCallbacks, TestLuaLRefRetainsCorrectReference) {
	const int testValue = 123456;
	int ref = 0, ud = 0;
	LuaGame game{ [&](lua_State* L, LuaInterface& lua) {
		if (ref == 0) {
			ud = testValue;
			*static_cast<int**>(lua.lua_newuserdata(L, sizeof(int))) = &ud;
			ref = lua.luaL_ref(L, LUA_REGISTRYINDEX);
		} else {
			if (ref == -2)
				return; //already tested but cleanup hasn't happened
			lua.lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			ASSERT_EQ(lua.lua_type(L, -1), LUA_TUSERDATA);
			EXPECT_EQ(**static_cast<int**>(lua.lua_touserdata(L, -1)),
				testValue);
			lua.lua_remove(L, -1);
			lua.luaL_unref(L, LUA_REGISTRYINDEX, ref);
			lua.lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			EXPECT_NE(lua.lua_type(L, -1), LUA_TUSERDATA);
			ref = -2;
			wasInvoked.set_value(true);
		}
		} };
	DoWaitTest(2s);
}

TEST_F(TestCallbacks, TestLuaPCallHandlesErrors) {
	LuaGame game{ [this](lua_State* L, LuaInterface& l) {
		l.lua_pushcclosure(L, [](lua_State* L, LuaInterface& l) {
			EXPECT_EQ(l.lua_gettop(L), 1);
			EXPECT_EQ(l.lua_type(L, 1), LUA_TSTRING);
			l.lua_pushboolean(L, true);
			return 1;
		}, 0);
		l.luaL_loadstring(L, "madeupvariable:x(5)");
		l.lua_pcall(L, 0, 0, -2);
		EXPECT_EQ(l.lua_type(L, -1), LUA_TBOOLEAN);
		EXPECT_EQ(l.lua_toboolean(L, -1), TRUE);
		wasInvoked.set_value(true);
} };
	DoWaitTest(2s);
}

TEST_F(TestCallbacks, TestLuaLoad) {
	LuaGame game{ [this](lua_State* L, LuaInterface& l) {
		const int testValue = 123456;
		int val = testValue;
		auto ret = l.lua_load(L, [](lua_State* L, void* ud, size_t* size) -> const char* {
			static bool wasLoaded = false;
			if (wasLoaded)
				return wasLoaded = false, *size = 0, nullptr;
			static const std::string s{ std::string{"return "} + std::to_string(*static_cast<int*>(ud)) };
			*size = s.length();
			wasLoaded = true;
			return s.c_str();
		}, &val, "TestLuaLoad");
		l.lua_call(L, 0, 0);
		EXPECT_EQ(ret, 0);
		wasInvoked.set_value(true);
	} };
	DoWaitTest(2s);
}

TEST_F(TestCallbacks, TestFunctionWithClosures) {
	LuaGame game{ [this](lua_State* L, LuaInterface& l) {
		l.lua_pushinteger(L, 123456);
		l.lua_pushboolean(L, true);
		l.lua_pushstring(L, "test");
		l.lua_pushnil(L);
		l.lua_pushcclosure(L, [](lua_State* L, LuaInterface& l) {
			const std::string testStr{ "test" };
			EXPECT_EQ(l.lua_tonumber(L, lua_upvalueindex(1)), 123456);
			EXPECT_EQ(l.lua_toboolean(L, lua_upvalueindex(2)), TRUE);
			EXPECT_EQ(l.lua_tostring(L, lua_upvalueindex(3)), testStr);
			EXPECT_EQ(l.lua_type(L, lua_upvalueindex(4)), LUA_TNIL);
			l.lua_pushinteger(L, 654321);
			return 1;
		}, 4);
		l.lua_call(L, 0, 1);
		EXPECT_EQ(l.lua_tointeger(L, -1), 654321);
		wasInvoked.set_value(true);
	} };
	DoWaitTest(2s);
}

TEST_F(TestCallbacks, TestLuaThreadYieldResumeAndXmove) {
	LuaGame game{ [this](lua_State* L, LuaInterface& l) {
		auto T = l.lua_newthread(L);
		EXPECT_EQ(l.luaL_loadstring(T, R"_(
		local num, f = ...
		return string.reverse(tostring(
		f(string.reverse(tostring(num)))
		))
		)_"), 0);
		l.lua_pushinteger(T, 123456);
		l.lua_pushcclosure(T, [](lua_State* L, LuaInterface& l) {
			EXPECT_EQ(l.lua_gettop(L), 1);
			return l.lua_yield(L, 1);
		}, 0);
		EXPECT_EQ(l.lua_resume(T, 2), LUA_YIELD);
		EXPECT_EQ(l.lua_tointeger(T, -1), 654321);
		l.lua_pushinteger(T, 987654);
		EXPECT_EQ(l.lua_resume(T, 1), 0);
		EXPECT_EQ(l.lua_tointeger(T, -1), 456789);
		l.lua_pushinteger(T, 987654);
		l.lua_xmove(T, L, 1);
		EXPECT_EQ(l.lua_tointeger(L, -1), 987654);
		wasInvoked.set_value(true);
	} };
	DoWaitTest(2s);
}

TEST_F(TestCallbacks, TestLuaTableIterationWorks) {
	LuaGame game{ [this](lua_State* L, LuaInterface& l) {
		EXPECT_EQ(l.luaL_loadstring(L, "return {9,8,7,6,5,4,3,2,1}"), 0);
		l.lua_call(L, 0, 1);
		EXPECT_EQ(l.lua_type(L, -1), LUA_TTABLE);
		l.lua_pushinteger(L, 10);
		l.lua_pushinteger(L, 0);
		l.lua_settable(L, -3);
		l.lua_pushnil(L);
		for (int i = 9; l.lua_next(L, -2); i--) {
			EXPECT_EQ(l.lua_tointeger(L, -1), i);
			l.lua_pop(L, 1);
		}
		wasInvoked.set_value(true);
	} };
	DoWaitTest(2s);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason,LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		{
			auto cmdLine = GetCommandLine();
			auto argc = 0;
			auto localFree = [](wchar_t** s) { LocalFree(s); };
			std::unique_ptr<wchar_t*, decltype(localFree)> argv{
				static_cast<wchar_t**>(CommandLineToArgvW(cmdLine, &argc)),
				localFree };
			::testing::InitGoogleTest(&argc, argv.get());
			CloseHandle(CreateThread(nullptr, 1024*1024, [](auto arg) {
				return static_cast<DWORD>(RUN_ALL_TESTS());
			}, nullptr, 0, nullptr));
			FILE* fp;
			freopen_s(&fp, "CONOUT$", "w", stdout);
			freopen_s(&fp, "CONOUT$", "w", stderr);
			freopen_s(&fp, "CONIN$", "r", stdin);
			std::ios::sync_with_stdio();
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

