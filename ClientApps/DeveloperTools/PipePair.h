#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace LuaHookNG {
	class PipePair {
		HANDLE r, w;

	public:
		PipePair();
		PipePair(PipePair&&);
		PipePair(const PipePair&) = delete;
		~PipePair();
		HANDLE read();
		HANDLE write();
	};
}  // namespace LuaHookNG
