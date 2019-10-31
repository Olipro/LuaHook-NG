if(WIN32)
	file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.10.2/catch.hpp ${CMAKE_BINARY_DIR}/catch.hpp)
	add_library(TestLuaHookNG SHARED
		CatchMain.cpp CatchMain.h
		TestInterception.cpp
		TestLuaAPI.cpp
		dllmain.cpp)
	set_target_properties(TestLuaHookNG PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES)
	target_include_directories(TestLuaHookNG PRIVATE ${CMAKE_BINARY_DIR})
	target_link_libraries(TestLuaHookNG LuaHookNG)
	install(TARGETS TestLuaHookNG RUNTIME DESTINATION lib/LuaHook-NG)
endif()
