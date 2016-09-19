#pragma once
#include "stdafx.h"

namespace Olipro {

	template <typename T, typename ...Args,
		typename R = std::result_of<T(Args...)>::type,
		typename = std::enable_if_t<!std::is_void<R>::value>
	>
	R SafeCall(T func, Args... args)
	{
		size_t savedEsp;
		__asm mov savedEsp, esp
		auto ret = func(std::forward<Args>(args)...);
		__asm mov esp, savedEsp
		return ret;
	}

	template <typename T, typename ...Args,
		typename R = std::result_of_t<T(Args...)>,
		typename = std::enable_if_t<std::is_void<R>::value>
	>
	void SafeCall(T func, Args... args)
	{
		size_t savedEsp;
		__asm mov savedEsp, esp
		func(std::forward<Args>(args)...);
		__asm mov esp, savedEsp
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
