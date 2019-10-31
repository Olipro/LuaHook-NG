#include "detail/LuaAppSelector.h"

#include <stdexcept>

#include "detail/windows/x86/PAYDAY1/Payday1Handler.h"
#include "detail/windows/x86/PAYDAY2/Payday2Handler.h"

using namespace LuaHookNG;

std::unique_ptr<ILuaAppHandler> ProduceAppHandler(
  const std::wstring_view& appName,
  IRuntimeCodeHook& hook) {
	if (appName.rfind(L"payday2_win32_release.exe") != appName.npos)
		return std::make_unique<Payday2Handler>(hook);
	else if (appName.rfind(L"payday_win32_release.exe") != appName.npos)
		return std::make_unique<Payday1Handler>(hook);
	throw std::runtime_error{"No supported executable name found."};
}

LuaAppSelector::LuaAppSelector(const std::wstring_view& appName,
							   IRuntimeCodeHook& hook,
							   ILibraryLoader& libLoader) :
  appHandler{ProduceAppHandler(appName, hook)} {
	libLoader.LoadClientLibraries();
}
