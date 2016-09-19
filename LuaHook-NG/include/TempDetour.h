#pragma once
#include "FunctionHooker.h"

namespace Olipro {
	class TempDetour {
		void** f;
		FunctionHooker::UnsafePtr* d;
	public:
		template <typename T, typename R>
		TempDetour(T* func, R detour, T* trampoline) : 
			f(reinterpret_cast<decltype(f)>(func)),
			d(reinterpret_cast<decltype(d)>(detour))
		{
			FunctionHooker::SingleAttach(func, detour, trampoline);
		}
		~TempDetour()
		{
			FunctionHooker::SingleDetach(f, d);
		}
	};
}
