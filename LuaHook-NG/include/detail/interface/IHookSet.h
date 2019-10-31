#pragma once

#include "detail/interface/IHooked.h"

namespace LuaHookNG {
	class IHookSet {
	public:
		virtual IHooked& operator[](size_t) noexcept = 0;
		virtual ~IHookSet()							 = default;
	};
}  // namespace LuaHookNG