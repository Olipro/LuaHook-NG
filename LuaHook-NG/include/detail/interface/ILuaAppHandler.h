#pragma once

#include <string>

namespace LuaHookNG {
	class ILuaAppHandler {
	public:
		virtual const std::string& HandledAppName() const noexcept = 0;
		virtual ~ILuaAppHandler()								   = default;
	};
}  // namespace LuaHookNG