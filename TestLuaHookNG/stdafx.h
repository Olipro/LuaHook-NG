// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "LuaGame.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ShellAPI.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <future>
#include <string>
#include <thread>

using namespace std::chrono_literals;

#pragma comment (lib, "gmock.lib")
