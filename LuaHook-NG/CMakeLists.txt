cmake_minimum_required (VERSION 3.8)
enable_language(C CXX)

configure_file(cmake/Version.h.in Version.h)

add_library(LuaHookNG SHARED
	src/LuaHookNG.cpp include/public/LuaHookNG.h
	src/LuaAppSelector.cpp include/detail/LuaAppSelector.h
	src/LuaAppLink.cpp include/public/LuaAppLink.h
	src/LuaAPI.cpp include/public/LuaAPI.h
	include/detail/LinkHandler.h
	include/detail/LuaAPIImpl.h
	include/detail/LuaAppSelector.h
	include/detail/RegistrationManager.h
	include/detail/interface/ILibraryLoader.h
	include/detail/interface/ILuaAppHandler.h
	include/detail/interface/IRuntimeCodeHook.h)
target_include_directories(LuaHookNG PRIVATE include include/public ${CMAKE_CURRENT_BINARY_DIR}
									 INTERFACE include/public)
set_target_properties(LuaHookNG PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	OUTPUT_NAME "LuaHook-NG")

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
	include(cmake/win32.cmake)
endif()

install(TARGETS LuaHookNG LIBRARY DESTINATION lib RUNTIME DESTINATION lib)
install(DIRECTORY include/public/ DESTINATION include)
