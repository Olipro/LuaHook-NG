#include "detail/windows/x86/PAYDAY1/Payday1Handler.h"

using namespace LuaHookNG;

Payday1Handler::Payday1Handler(IRuntimeCodeHook&) {}

const std::string& Payday1Handler::HandledAppName() const noexcept {
	return handledAppName;
}