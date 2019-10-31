#include <atomic>
#include <catch.hpp>
#include <chrono>
#include <future>
#include <iostream>

#include "LuaHookNG.h"

using namespace LuaHookNG;
using namespace std::literals;

TEST_CASE("OnNewState & OnRequire Interception works properly") {
	struct OnRequireHandler : public IOnRequireClient {
		std::promise<std::string> p;
		bool done = false;
		void OnRequire(lua_State*, std::string_view name) override {
			if (!done) {
				p.set_value(std::string{name});
				done = true;
			}
		}
	} reqHandler;
	struct OnNewStateHandler : public INewStateClient {
		std::promise<void> p;
		bool done = false;
		void OnNewState(lua_State*) override {
			if (!done) {
				p.set_value();
				done = true;
			}
		}
	} handler;
	LuaAppLink appLink{handler}, reqLink{reqHandler};
	CHECK(handler.p.get_future().wait_for(2s) == std::future_status::ready);
	auto&& fut = reqHandler.p.get_future();
	REQUIRE(fut.wait_for(7s) == std::future_status::ready);
	CHECK(fut.get().find("core/lib/system/") != std::string::npos);
}

TEST_CASE("OnTick Interception works properly") {
	struct OnTickHandler : public IOnTickClient {
		std::promise<void> p;
		void OnTick(lua_State*) override {
			p.set_value();
		}
	} handler;
	LuaAppLink appLink{handler};
	CHECK(handler.p.get_future().wait_for(5s) == std::future_status::ready);
}

TEST_CASE("OnTick Interception removal works properly") {
	struct OnTickHandler : public IOnTickClient {
		std::atomic_int_fast32_t ctr = 0;
		std::promise<void> p, q;
		void OnTick(lua_State*) override {
			++ctr;
			q.set_value();
			p.get_future().get();
		}
	} handler;
	{
		LuaAppLink appLink{handler};
		CHECK(handler.q.get_future().wait_for(2s) == std::future_status::ready);
		handler.p.set_value();
	}
	std::this_thread::sleep_for(60ms);
	CHECK(handler.ctr == 1);
}

TEST_CASE("OnCloseState Interception works properly") {
	struct OnTickHandler : public IOnTickClient, public ICloseStateClient {
		std::promise<void> p;
		void OnTick(lua_State*) override {
			auto L = LuaHookNG::luaL_newstate();
			LuaHookNG::lua_close(L);
		}
		void OnCloseState(lua_State*) override {
			p.set_value();
		}
	} handler;
	auto appLinks = LuaAppLink::RegisterMultiple(handler);
	CHECK(handler.p.get_future().wait_for(2s) == std::future_status::ready);
}
