#pragma once
#include "stdafx.h"

namespace Olipro {
	//Functions implemented in UtilityAsm
	void __fastcall SaveEsp(size_t*);
	void __fastcall LoadEsp(size_t);

	template <typename T>
	using enable_if_void = std::enable_if_t<std::is_void<T>::value>;
	template <typename T>
	using disable_if_void = std::enable_if_t<!std::is_void<T>::value, T>;

	template <typename T, typename ...Args,
		typename R = disable_if_void<std::result_of<T(Args...)>::type>>
	R SafeCall(T func, Args... args)
	{
		size_t savedEsp;
		SaveEsp(&savedEsp);
		auto ret = func(std::forward<Args>(args)...);
		LoadEsp(savedEsp);
		return ret;
	}

	template <typename T, typename ...Args,
		typename = enable_if_void<std::result_of_t<T(Args...)>>>
	void SafeCall(T func, Args... args)
	{
		size_t savedEsp;
		SaveEsp(&savedEsp);
		func(std::forward<Args>(args)...);
		LoadEsp(savedEsp);
	}

	constexpr auto Str2Uint(LPCSTR str)
	{
		return *reinterpret_cast<const int*> (str);
	}
}
