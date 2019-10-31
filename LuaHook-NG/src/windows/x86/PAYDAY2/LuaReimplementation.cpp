#include "detail/windows/x86/PAYDAY2/LuaReimplementation.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string_view>

using namespace LuaHookNG;
using namespace std::literals;

void LuaReimplementation::Init(IRuntimeCodeHook& codeHook) {
	index2adr = codeHook.GetFuncRef(
	  index2adr,
	  "\x8B\x4C\x24\x08\x85\xC9\x7E\x1F\x8B\x54\x24\x04\x8B\x42\x10\x8D"sv,
	  0x62E150);
	lj_obj_equal = codeHook.GetFuncRef(
	  lj_obj_equal,
	  "\x8B\x41\x04\x3B\x42\x04\x75\x1A\x83\xF8\xFD\x72\x06\xB8\x01\x00"sv,
	  0x61B7F0);
	lj_obj_ptr = codeHook.GetFuncRef(
	  lj_obj_ptr,
	  "\x8B\x41\x04\x83\xF8\xF3\x75\x06\x8B\x01\x83\xC0\x18\xC3\x83\xF8"sv,
	  0x61B830);
	lj_gc_check = codeHook.GetFuncRef(
	  lj_gc_check,
	  "\x83\xEC\x08\x53\x8B\x59\x08\x55\x8B\x43\x4C\x8B\x53\x18\x89\x44"sv,
	  0x61B290);
	lj_strfmt_pushvf = codeHook.GetFuncRef(
	  lj_strfmt_pushvf,
	  "\x55\x8B\xEC\x83\xE4\xC0\x8B\x55\x0C\x83\xEC\x34\x8B\xCA\x53\x8B"sv,
	  0x63A210);
	lj_bcwrite = codeHook.GetFuncRef(
	  lj_bcwrite,
	  "\x83\xEC\x28\xA1\x74\xED\xA2\x00\x33\xC4\x89\x44\x24\x24\x8B\x44"sv,
	  0x631E00);
	luaL_loadfilex = codeHook.GetFuncRef(
	  luaL_loadfilex,
	  "\x81\xEC\x08\x02\x00\x00\xA1\x74\xED\xA2\x00\x33\xC4\x89\x84\x24"sv,
	  0x63E5D0);
}

int LuaReimplementation::lua_iscfunction(lua_State* L, int n) {
	auto* tVal = index2adr(L, n);
	return tVal[1] == -9 && *reinterpret_cast<int8_t*>(*tVal + 6);
}

int LuaReimplementation::lua_rawequal(lua_State* L, int a, int b) {
	auto* lState = reinterpret_cast<uint32_t*>(L);
	auto* aAdr	 = index2adr(L, a);
	auto* bAdr	 = index2adr(L, b);
	auto* cmp	 = reinterpret_cast<int*>(lState[2] + 144);
	return (aAdr == cmp || bAdr == cmp) ? 0 : lj_obj_equal(aAdr, bAdr);
}

lua_CFunction LuaReimplementation::lua_tocfunction(lua_State* L, int idx) {
	auto* adr = index2adr(L, idx);
	int v3	  = *adr;
	auto v4	  = **reinterpret_cast<uint32_t**>(v3 + 16) & 0xFF;
	if (adr[1] != -9 || (v4 != 95 && v4 != 96))
		return 0;
	return *reinterpret_cast<lua_CFunction*>(v3 + 20);
}

void* LuaReimplementation::lua_touserdata(lua_State* L, int idx) {
	auto* adr = index2adr(L, idx);
	auto type = adr[1];
	switch (type) {
		case -13:
			return reinterpret_cast<void*>(*adr + 24);
		case -4:
			return reinterpret_cast<void*>(*adr);
	}
	return nullptr;
}

lua_State* LuaReimplementation::lua_tothread(lua_State* L, int idx) {
	auto* adr = index2adr(L, idx);
	return adr[1] == -7 ? reinterpret_cast<lua_State*>(*adr) : nullptr;
}

const void* LuaReimplementation::lua_topointer(lua_State* L, int idx) {
	return lj_obj_ptr(index2adr(L, idx));
}

const char* LuaReimplementation::lua_pushvfstring(lua_State* L,
												  const char* fmt,
												  va_list args) {
	lj_gc_check(L);
	return lj_strfmt_pushvf(L, fmt, args);
}

int LuaReimplementation::lua_dump(lua_State* L, lua_Writer writer, void* data) {
	auto* o = *reinterpret_cast<uint32_t**>(reinterpret_cast<char*>(L) + 20);
	if (o[-1] != -9)
		return 1;
	auto v4 = o[-2];
	if (*reinterpret_cast<char*>(v4 + 6))
		return 1;
	return lj_bcwrite(L,
					  *reinterpret_cast<uint32_t**>(v4 + 16) - 16,
					  writer,
					  data,
					  0);
}

int LuaReimplementation::lua_status(lua_State* L) {
	return reinterpret_cast<char*>(L)[7];
}

int LuaReimplementation::luaL_loadfile(lua_State* L, const char* filename) {
	return luaL_loadfilex(L, filename, 0);
}

lua_State* LuaReimplementation::luaL_newstate() {
	auto alloc = [](void*, void* ptr, size_t, size_t nsize) -> void* {
		if (nsize == 0)
			return free(ptr), nullptr;
		return realloc(ptr, nsize);
	};
	return lua_newstate(alloc, nullptr);
}

int LuaReimplementation::NOTIMPL_lua_cpcall(lua_State*, lua_CFunction, void*) {
	MessageBox(nullptr,
			   L"lua_cpcall not available for PAYDAY2. Please use only "
			   L"lua_call or lua_pcall in your mod.",
			   L"Unimplemented Lua API function",
			   0);
	throw std::runtime_error{"lua_cpcall is not available for PAYDAY2"};
}

int LuaReimplementation::NOTIMPL_lua_yield(lua_State*, int) {
	MessageBox(nullptr,
			   L"lua_yield not available for PAYDAY2. Function was not found "
			   L"in the binary.",
			   L"Unimplemented Lua API function",
			   0);
	throw std::runtime_error{"lua_yield is not available for PAYDAY2"};
}
