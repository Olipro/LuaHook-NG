#include "LuaAppLink.h"

#include <shared_mutex>
#include <vector>

#include "detail/LinkHandler.h"

using namespace LuaHookNG;

LuaAppLink::LuaAppLink(INewStateClient& client) :
  handler{new LinkHandler{client}} {}
LuaAppLink::LuaAppLink(ICloseStateClient& client) :
  handler{new LinkHandler{client}} {}
LuaAppLink::LuaAppLink(IOnTickClient& client) :
  handler{new LinkHandler{client}} {}
LuaAppLink::LuaAppLink(IOnRequireClient& client) :
  handler{new LinkHandler{client}} {}

LuaAppLink::LuaAppLink(LuaAppLink&&) = default;

LuaAppLink& LuaAppLink::operator=(LuaAppLink&&) = default;

LuaAppLink::~LuaAppLink() = default;
