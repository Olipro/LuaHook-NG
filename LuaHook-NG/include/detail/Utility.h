#pragma once

#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	class SendTo : public IInterceptor {
		void* func;

	public:
		template<typename Func>
		SendTo(Func func) : func{reinterpret_cast<void*>(func)} {}

		void* Address() const noexcept override {
			return func;
		}
	};

	template<typename T>
	class AnyCall {
		T func;

	public:
		AnyCall() = default;
		AnyCall(T) {}
		AnyCall(void* ptr) : func{reinterpret_cast<T>(ptr)} {}

		template<typename... Args>
		auto operator()(Args&&... args) {
			return func(std::forward<Args>(args)...);
		}
	};
}  // namespace LuaHookNG
