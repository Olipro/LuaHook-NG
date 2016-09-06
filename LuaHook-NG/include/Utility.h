#pragma once
#include "stdafx.h"

namespace Olipro {

	class StackGuard
	{
		size_t esp = 0;
	public:
		__forceinline StackGuard() {
			__asm mov [this]StackGuard.esp, esp
		}
		__forceinline ~StackGuard()
		{
			__asm mov esp, [this]StackGuard.esp
		}
	};

	template <typename T, typename ...Args>
	__forceinline auto SafeCall(T func, Args... args)
	{
		StackGuard guard;
		return func(args...);
	}

	constexpr auto Str2Uint(LPCSTR str)
	{
		return *reinterpret_cast<const int32_t*>(str);
	}

	template <typename T>
	class CompileTimeArray
	{
		typedef std::function<T(size_t elem)> get_t;
		const get_t GetElement;
		template <typename T, size_t ...args>
		constexpr T FillArray(std::index_sequence<args...>, T) const
		{
			return{ GetElement(args)... };
		}
	public:
		constexpr CompileTimeArray(get_t getter) : GetElement(getter) { }
		template <typename T>
		constexpr T FillArray(T type) const
		{
			return FillArray(std::make_index_sequence<type.max_size()>(), x);
		}
	};
}
