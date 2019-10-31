#include "detail/windows/x86/PAYDAY2/OnNewStateHandler.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_set>

#include "LuaAPI.h"
#include "detail/LinkHandler.h"
#include "detail/Utility.h"
#include "detail/windows/x86/AsmUtils.h"
#include "interface/INewStateClient.h"
#include "interface/IOnRequireClient.h"

using namespace LuaHookNG;
using namespace std::literals;

static void __fastcall LuaHookNG::OnNewState(lua_State** L,
								  void* edx,
								  void* a,
								  void* b,
								  void* c);

static std::unique_ptr<IHooked> hook;
static std::unique_ptr<IHookable> luaSetField;
static IRuntimeCodeHook* codeHook;
static AnyCall<void(__cdecl*)(lua_State*, int, const char*)> setFieldTrampoline;
static std::unordered_set<std::string> packageLoaded;
static AnyCall trampoline{&OnNewState};

static void OnRequire(lua_State* L, std::string_view name) {
	LinkHandler<IOnRequireClient>::CallClients([L, &name](auto& client) {
		client->OnRequire(L, name);
	});
}

static void HookLuaRequire(lua_State* L) {
	lua_getfield(L, LUA_GLOBALSINDEX, "require");  // upvalue for cclosure.
	lua_pushcclosure(
	  L,
	  [](auto L) {
		  size_t strLen;
		  auto reqStr = lua_tolstring(L, 1, &strLen);
		  std::string name{reqStr, strLen};
		  lua_pushvalue(L, lua_upvalueindex(1));
		  lua_insert(L, 1);
		  lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
		  if (packageLoaded.find(name) == packageLoaded.end()) {
			  packageLoaded.insert(name);
			  lua_pushcclosure(
				L,
				[](auto L) {
					auto& name =
					  *static_cast<std::string*>(lua_touserdata(L, 1));
					return OnRequire(L, name), 0;
				},
				0);
			  lua_pushlightuserdata(L, &name);
			  lua_call(L, 1, 0);
		  }
		  return lua_gettop(L);
	  },
	  1);
	lua_setfield(L, LUA_GLOBALSINDEX, "require");
	lua_pcall(L, 0, 0, 0);
}

void OnNewStateHandler::Init(IRuntimeCodeHook& cHook) {
	codeHook	= &cHook;
	luaSetField = cHook.FindFunction(
	  "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x10\x56\x8B\x75\x08\x57\xFF\x75"sv);
	hook = cHook.Hook(
	  cHook.FindFunction(
		"\x53\x56\x33\xDB\x57\x8B\xF1\x39\x5C\x24\x18\x0F\x84\x56\x01"sv),
	  SendTo{&OnNewState});
	trampoline = hook->TrampolineAddress();
}

void __fastcall LuaHookNG::OnNewState(lua_State** L,
						   void* edx,
						   void* a,
						   void* b,
						   void* c) {
	{
		auto setFieldHook =
		  codeHook->Hook(luaSetField,
						 SendTo{&OnNewStateHandler::SetFieldHandler_ASM});
		setFieldTrampoline = setFieldHook->TrampolineAddress();
		trampoline(L, edx, a, b, c);
	}
	LinkHandler<INewStateClient>::CallClients([L](auto& client) {
		client->OnNewState(*L);
	});
	packageLoaded.clear();
	HookLuaRequire(*L);
}

void OnNewStateHandler::SetFieldHandler(lua_State* L,
										int tbl,
										const char* field) {
	std::string_view name = field;
	if (tbl == LUA_GLOBALSINDEX && (name == "xpcall" || name == "pcall"))
		lua_settop(L, -2);
	else
		SafeCall(setFieldTrampoline, L, tbl, field);
}
