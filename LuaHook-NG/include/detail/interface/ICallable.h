#pragma once

namespace LuaHookNG {
	class ICallable {
	public:
		virtual void* Address() const noexcept = 0;
		virtual ~ICallable()				   = default;
	};
}  // namespace LuaHookNG
