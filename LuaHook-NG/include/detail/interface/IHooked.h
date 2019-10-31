#pragma once

#include "detail/interface/ICallable.h"

namespace LuaHookNG {
	class IHooked : public ICallable {
	public:
		virtual void* TrampolineAddress() const noexcept = 0;
	};
}  // namespace LuaHookNG
