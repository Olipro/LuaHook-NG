#pragma once

namespace LuaHookNG {
	class ILinkHandler {
	public:
		virtual ~ILinkHandler() = 0;
	};

	inline ILinkHandler::~ILinkHandler() = default;
}  // namespace LuaHookNG
