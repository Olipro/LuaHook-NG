#pragma once
#include "Detours.h"

namespace Olipro {
	class FunctionHooker
	{
		bool isCommitted = false, isAborted = false;
	public:
		struct UnsafePtr;

		template<typename T, typename R>
		static bool SingleAttach(T* target, R detour, T* trampoline)
		{
			FunctionHooker self;
			return self.Attach(target, detour, trampoline);
		}

		template <typename T, typename R>
		static bool SingleDetach(T* target, R detour)
		{
			FunctionHooker self;
			return self.Detach(target, detour);
		}

		FunctionHooker& operator=(const FunctionHooker&) = delete;
		FunctionHooker& operator=(FunctionHooker&& other) = delete;
		FunctionHooker();
		~FunctionHooker();
		bool Commit();
		void Abort();

		template <typename T>
		bool Attach(T* target, UnsafePtr* detour, T* trampoline)
		{
			return DetourAttachEx(
				reinterpret_cast<void**>(target),
				detour,
				reinterpret_cast<PDETOUR_TRAMPOLINE*>(trampoline),
				nullptr,
				nullptr) == NO_ERROR;
		}

		template <typename T>
		bool Attach(T* target, T detour, T* trampoline)
		{
			return Attach(	target, reinterpret_cast<UnsafePtr*>(detour),
							trampoline);
		}

		template <typename T>
		bool Detach(T* target, UnsafePtr* detour)
		{
			return DetourDetach(reinterpret_cast<void**>(target), detour)
				== NO_ERROR;
		}
		template <typename T>
		bool Detach(T* target, T detour)
		{
			return Detach(target, reinterpret_cast<UnsafePtr*>(detour));
		}
	};
}