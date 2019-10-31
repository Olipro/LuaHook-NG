#pragma once
#include <memory>
#include <string_view>

#include "detail/interface/ILibraryLoader.h"
#include "detail/interface/ILuaAppHandler.h"
#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	class LuaAppSelector {
	public:
		LuaAppSelector(const std::wstring_view& appName,
					   IRuntimeCodeHook& hook,
					   ILibraryLoader& libLoader);

	private:
		std::unique_ptr<ILuaAppHandler> appHandler;
	};
}  // namespace LuaHookNG
