#include "detail/windows/x86/PAYDAY2/OnCloseStateHandler.h"

#include <string_view>

#include "detail/LinkHandler.h"
#include "detail/Utility.h"
#include "interface/ICloseStateClient.h"

using namespace LuaHookNG;
using namespace std::literals;

static void __cdecl OnCloseState(lua_State* L);

static AnyCall trampoline{&OnCloseState};
static std::unique_ptr<IHooked> hook;

static void __cdecl OnCloseState(lua_State* L) {
	LinkHandler<ICloseStateClient>::CallClients([L](auto& client) {
		client->OnCloseState(L);
	});
	trampoline(L);
}

void OnCloseStateHandler::Init(IRuntimeCodeHook& codeHook) {
	hook = codeHook.Hook(
	  codeHook.FindFunction(
		"\x8B\x44\x24\x04\x53\x56\x57\x8B\x78\x08\x8B\x77\x74\x56\xE8\x6D"sv),
	  SendTo{&OnCloseState});
	trampoline = hook->TrampolineAddress();
}
