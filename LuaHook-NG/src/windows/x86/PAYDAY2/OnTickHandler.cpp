#include "detail/windows/x86/PAYDAY2/OnTickHandler.h"

#include <string_view>

#include "detail/LinkHandler.h"
#include "detail/Utility.h"
#include "interface/IOnTickClient.h"

using namespace LuaHookNG;
using namespace std::literals;

static std::unique_ptr<IHooked> hook;

void OnTickHandler::Init(IRuntimeCodeHook& codeHook) {
	hook = codeHook.Hook(
	  codeHook.FindFunction("\x56\xFF\x74\x24\x0C\x8B\xF1\x68\xEE\xD8"sv),
	  SendTo{&OnGameTick_ASM});
	SetOnGameTickTrampoline_ASM(hook->TrampolineAddress());
}

void OnTickHandler::OnGameTick(lua_State* L, const char* op) {
	if (std::string_view{op, 4} == "upda")
		LinkHandler<IOnTickClient>::CallClients([L](auto& client) {
			client->OnTick(L);
		});
}
