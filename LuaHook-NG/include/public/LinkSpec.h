#pragma once

#ifndef DLLSPEC
	#ifdef _WIN32
		#pragma comment(lib, "LuaHook-NG.lib")
		#define DLLSPEC __declspec(dllimport)
	#endif
#endif
