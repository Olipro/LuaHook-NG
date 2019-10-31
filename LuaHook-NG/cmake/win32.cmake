include(ExternalProject)
enable_language(C CXX ASM_MASM)

ExternalProject_Add(DetoursSrc
	GIT_REPOSITORY https://github.com/microsoft/Detours.git
	GIT_TAG master
	GIT_SHALLOW YES
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	TEST_COMMAND ""
	BINARY_DIR ${CMAKE_BINARY_DIR}/Detours-Build
)
ExternalProject_Get_Property(DetoursSrc SOURCE_DIR)
set(DETOURS_SOURCES
	${CMAKE_BINARY_DIR}/Detours-Build/detours.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/modules.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/disasm.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/image.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/creatwth.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/disolx86.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/disolx64.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/disolia64.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/disolarm.cpp
	${CMAKE_BINARY_DIR}/Detours-Build/disolarm64.cpp)
add_custom_command(OUTPUT ${DETOURS_SOURCES}
	               COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/src/ ${CMAKE_BINARY_DIR}/Detours-Build
				   DEPENDS DetoursSrc)
add_library(Detours OBJECT ${DETOURS_SOURCES})
target_include_directories(Detours PUBLIC Detours-Build/)
add_dependencies(Detours DetoursSrc)
target_sources(LuaHookNG PRIVATE
	src/windows/DetoursCodeHook.cpp include/detail/windows/DetoursCodeHook.h
	src/windows/WindowsLibraryLoader.cpp include/detail/windows/WindowsLibraryLoader.h
	src/windows/x86/PAYDAY1/Payday1Handler.cpp include/detail/windows/x86/PAYDAY1/Payday1Handler.h
	src/windows/x86/PAYDAY2/Payday2Handler.cpp include/detail/windows/x86/PAYDAY2/Payday2Handler.h
	src/windows/x86/PAYDAY2/OnTickHandler.cpp include/detail/windows/x86/PAYDAY2/OnTickHandler.h
	src/windows/x86/PAYDAY2/OnNewStateHandler.cpp include/detail/windows/x86/PAYDAY2/OnNewStateHandler.h
	src/windows/x86/PAYDAY2/OnCloseStateHandler.cpp include/detail/windows/x86/PAYDAY2/OnCloseStateHandler.h
	src/windows/x86/PAYDAY2/LuaReimplementation.cpp include/detail/windows/x86/PAYDAY2/LuaReimplementation.h
	src/windows/x86/PAYDAY2/Payday2.asm
	src/windows/x86/AsmUtils.asm
	src/windows/dllmain.cpp
	include/detail/windows/DetoursCodeHook.h
	include/detail/windows/WindowsLibraryLoader.h
	include/detail/windows/x86/AsmUtils.h
	$<TARGET_OBJECTS:Detours>)
target_include_directories(LuaHookNG PRIVATE ${SOURCE_DIR}/src)
target_compile_definitions(LuaHookNG PUBLIC "DLLSPEC=__declspec(dllexport)" UNICODE)