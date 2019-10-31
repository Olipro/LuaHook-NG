#pragma once

#include <utility>

namespace LuaHookNG {
	void __fastcall SaveEsp(size_t*);
	void __fastcall LoadEsp(size_t);

	class ESPProtector {
		size_t savedEsp;

	public:
		__forceinline ESPProtector() {
			SaveEsp(&savedEsp);
		}
		__forceinline ~ESPProtector() {
			LoadEsp(savedEsp);
		}
	};
	template<typename T, typename... Args>
	auto SafeCall(T func, Args&&... args) {
		ESPProtector prot;
		return func(std::forward<Args>(args)...);
	}
}  // namespace LuaHookNG
