#pragma once

#include "detail/interface/ILibraryLoader.h"

namespace LuaHookNG {
	class WindowsLibraryLoader : public ILibraryLoader {
	public:
		void LoadClientLibraries() override;
	};
}  // namespace LuaHookNG
