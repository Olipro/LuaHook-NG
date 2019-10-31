cmake_minimum_required (VERSION 3.8)

include(CPack)

set(RELEASE_VERSION "1.0.0" CACHE STRING "Release version in format x.y.z")
project ("LuaHook-NG" LANGUAGES C CXX VERSION ${RELEASE_VERSION})

add_subdirectory("LuaHook-NG")
add_subdirectory("Hooks/HookIPHLPAPI")

add_subdirectory("ClientApps/DeveloperTools")
add_subdirectory("ClientApps/TestLuaHookNG")
