#include "detail/windows/DetoursCodeHook.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>

#include <stdexcept>
#include <string>

using namespace LuaHookNG;

using VecOfHooks = IRuntimeCodeHook::VecOfHooks;

static std::string_view FindModuleBase() {
	auto ptr = reinterpret_cast<const char*>(GetModuleHandle(nullptr));

	if (ptr) {
		auto dosHdr = reinterpret_cast<const IMAGE_DOS_HEADER*>(ptr);
		if (dosHdr->e_magic == IMAGE_DOS_SIGNATURE) {
			auto ntHdr =
			  reinterpret_cast<const IMAGE_NT_HEADERS*>(ptr + dosHdr->e_lfanew);
			auto& optHdr = ntHdr->OptionalHeader;
			return {ptr + optHdr.BaseOfCode, optHdr.SizeOfCode};
		}
	}
	throw std::runtime_error{"Unable to locate PE header"};
}

class HookableFunc : public IHookable {
	void* addr;

public:
	HookableFunc(void* addr) : addr{addr} {}
	void* Address() const noexcept override {
		return addr;
	}
};

class HookedFunc : public IHooked {
protected:
	void* targetAddr;
	void* detourAddr;
	PDETOUR_TRAMPOLINE trampoline;

public:
	HookedFunc(void* targetAddr,
			   void* detourAddr,
			   PDETOUR_TRAMPOLINE trampoline) :
	  targetAddr{targetAddr}, detourAddr{detourAddr}, trampoline{trampoline} {}

	void* Address() const noexcept override {
		return targetAddr;
	}

	void* TrampolineAddress() const noexcept override {
		return trampoline;
	}

	void* DetourAddress() const noexcept {
		return detourAddr;
	}
};

class SingleHook : public HookedFunc {
public:
	SingleHook(void* tAddr, void* dtAddr) : HookedFunc{tAddr, dtAddr, nullptr} {
		if (DetourTransactionBegin() != NO_ERROR)
			throw std::runtime_error{
			  "Unable to Begin Transaction for single item"};
		DetourUpdateThread(GetCurrentThread());
		if (DetourAttachEx(&targetAddr,
						   dtAddr,
						   &trampoline,
						   nullptr,
						   nullptr) != NO_ERROR) {
			DetourTransactionAbort();
			throw std::runtime_error{"Unable to attach single address"};
		}
		if (DetourTransactionCommit() != NO_ERROR) {
			DetourTransactionAbort();
			throw std::runtime_error{
			  "Unable to commit transaction for single item"};
		}
	}

	~SingleHook() override {
		DetourTransactionBegin();
		DetourDetach(&targetAddr, detourAddr);
		DetourTransactionCommit();
	}
};

class HookSet : public IHookSet {
	std::vector<HookedFunc> hooks;

public:
	HookSet(VecOfHooks&& toHook) {
		hooks.reserve(toHook.size());
		if (DetourTransactionBegin() != NO_ERROR)
			throw std::runtime_error{"Unable to Begin Transaction"};
		DetourUpdateThread(GetCurrentThread());
		for (auto&& i : toHook) {
			auto addr = i.first->Address();
			auto detr = i.second.Address();
			PDETOUR_TRAMPOLINE trampoline;
			if (DetourAttachEx(&addr, detr, &trampoline, nullptr, nullptr) !=
				NO_ERROR) {
				DetourTransactionAbort();
				throw std::runtime_error{"Unable to attach to address"};
			}
			hooks.emplace_back(addr, detr, trampoline);
		}
		if (DetourTransactionCommit() != NO_ERROR) {
			DetourTransactionAbort();
			throw std::runtime_error{"Transaction Commit failed."};
		}
	}

	~HookSet() override {
		DetourTransactionBegin();
		for (auto&& hook : hooks) {
			auto addr = hook.Address();
			DetourDetach(&addr, hook.DetourAddress());
		}
		DetourTransactionCommit();
	}

	IHooked& operator[](size_t n) noexcept override {
		return hooks[n];
	}
};

DetoursCodeHook::DetoursCodeHook() : moduleBase{FindModuleBase()} {}

std::unique_ptr<IHookable> DetoursCodeHook::FindFunction(
  std::string_view bytes) const {
	auto result = moduleBase.find(bytes);
	if (result == std::string_view::npos)
		return {};
	return std::make_unique<HookableFunc>(
	  const_cast<char*>(moduleBase.data() + result));
}

std::unique_ptr<IHookSet> DetoursCodeHook::Hook(VecOfHooks&& toHook) {
	return std::make_unique<HookSet>(std::move(toHook));
}

std::unique_ptr<IHooked> DetoursCodeHook::Hook(
  const std::unique_ptr<IHookable>& target,
  const IInterceptor& detour) {
	return std::make_unique<SingleHook>(target->Address(), detour.Address());
}
