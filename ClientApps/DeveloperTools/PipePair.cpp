#include "PipePair.h"

using namespace LuaHookNG;

PipePair::PipePair() {
	CreatePipe(&r, &w, nullptr, 0);
}

PipePair::PipePair(PipePair&& other) {
	r		= other.r;
	w		= other.w;
	other.r = other.w = nullptr;
}

PipePair::~PipePair() {
	if (r)
		CloseHandle(r);
	if (w)
		CloseHandle(w);
}

HANDLE PipePair::read() {
	return r;
}

HANDLE PipePair::write() {
	return w;
}