#include "LuaHookNG.h"

#include "Version.h"

void LuaHookNG::LuaHook_NG() {}

LuaHookNG::LuaHookVersion LuaHookNG::Version() {
	return {PROJECT_VERSION_MAJOR,
			PROJECT_VERSION_MINOR,
			PROJECT_VERSION_PATCH};
}
