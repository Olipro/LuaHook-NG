#pragma once

#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	class DetoursCodeHook : public IRuntimeCodeHook {
		std::string_view moduleBase;

	public:
		DetoursCodeHook();

		std::unique_ptr<IHookable> FindFunction(
		  std::string_view bytes) const override;

		std::unique_ptr<IHookSet> Hook(VecOfHooks&& toHook) override;

		std::unique_ptr<IHooked> Hook(const std::unique_ptr<IHookable>& target,
									  const IInterceptor& detour) override;
	};
}  // namespace LuaHookNG
