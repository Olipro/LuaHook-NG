#include "detail/windows/WindowsLibraryLoader.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <filesystem>

using namespace LuaHookNG;
namespace fs = std::filesystem;

void WindowsLibraryLoader::LoadClientLibraries() {
	for (auto&& file : fs::directory_iterator{"LuaHookNG"})
		if (file.is_regular_file())
			LoadLibraryW(file.path().wstring().c_str());
}