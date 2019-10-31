if (WIN32)
	add_library(DeveloperTools SHARED
		ConsoleManager.cpp ConsoleManager.h
		DeveloperTools.cpp DeveloperTools.h
		PipePair.cpp PipePair.h
		AutoJoinThread.h
		CyclicFuture.h)
	set_target_properties(DeveloperTools PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES)
	target_link_libraries(DeveloperTools LuaHookNG)
	install(TARGETS DeveloperTools RUNTIME DESTINATION lib/LuaHook-NG)
endif()
