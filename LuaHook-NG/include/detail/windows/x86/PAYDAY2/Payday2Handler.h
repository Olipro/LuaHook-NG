#pragma once

#include "detail/LuaAPIImpl.h"
#include "detail/interface/ILuaAppHandler.h"
#include "detail/interface/IRuntimeCodeHook.h"

namespace LuaHookNG {
	class Payday2Handler : public ILuaAppHandler {
		static inline const std::string handledAppName = "PAYDAY2";
		LuaHookNG::LuaFuncPtrs api;

	public:
		Payday2Handler(IRuntimeCodeHook&);
		const std::string& HandledAppName() const noexcept override;
	};
}  // namespace LuaHookNG
