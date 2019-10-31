#pragma once

#include "detail/interface/ILuaAppHandler.h"
#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	class Payday1Handler : public ILuaAppHandler {
		static inline const std::string handledAppName = "PAYDAY";

	public:
		Payday1Handler(IRuntimeCodeHook&);
		const std::string& HandledAppName() const noexcept override;
	};
}  // namespace LuaHookNG