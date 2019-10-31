#pragma once

#include <fstream>
#include <memory>
#include <string_view>
#include <vector>

#include "detail/interface/IHookSet.h"
#include "detail/interface/IHookable.h"
#include "detail/interface/IHooked.h"
#include "detail/interface/IInterceptor.h"

namespace LuaHookNG {
	class IRuntimeCodeHook {
	public:
		using VecOfHooks =
		  std::vector<std::pair<std::unique_ptr<IHookable>, IInterceptor&>>;

		template<typename Signature, typename... Args>
		static auto Call(ICallable& callable, Args&&... args) {
			auto f = static_cast<Signature>(callable.Address());
			return f(std::forward<Args>(args)...);
		}

		template<typename Signature>
		constexpr Signature& GetFuncRef(Signature*,
										std::string_view bytes,
										size_t p) {
			auto ret = FindFunction(bytes)->Address();
			if (p)
				if (ret != reinterpret_cast<void*>(p))
					std::ofstream{"mismatches.txt", std::ios::app}
					  << "got: " << ret
					  << " expected: " << reinterpret_cast<void*>(p) << '\n';
			return *reinterpret_cast<Signature*>(ret);
		}

		virtual std::unique_ptr<IHookable> FindFunction(
		  std::string_view bytes) const							   = 0;
		virtual std::unique_ptr<IHookSet> Hook(VecOfHooks&&)	   = 0;
		virtual std::unique_ptr<IHooked> Hook(const std::unique_ptr<IHookable>&,
											  const IInterceptor&) = 0;
	};
}  // namespace LuaHookNG
