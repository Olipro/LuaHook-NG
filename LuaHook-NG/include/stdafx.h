// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <fcntl.h>
#include <io.h>
#include <Shlobj.h>
#include <windows.h>

#include "lapi.h"
#include "lstate.h"
#include "lauxlib.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdarg>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

using namespace std::literals;

#ifdef LUAHOOKNG_EXPORTS
#define EXTERNAL __declspec(dllexport)
#else
#define EXTERNAL __declspec(dllimport)
#endif

#pragma comment(lib, "Detours.lib")
#pragma comment(lib, "Lua5.1.lib")
