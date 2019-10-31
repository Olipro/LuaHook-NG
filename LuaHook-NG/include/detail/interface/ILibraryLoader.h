#pragma once

namespace LuaHookNG {
	class ILibraryLoader {
	public:
		virtual void LoadClientLibraries() = 0;
		virtual ~ILibraryLoader()		   = default;
	};
}  // namespace LuaHookNG
