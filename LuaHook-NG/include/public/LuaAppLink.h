#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "LinkSpec.h"
#include "interface/ICloseStateClient.h"
#include "interface/INewStateClient.h"
#include "interface/IOnRequireClient.h"
#include "interface/IOnTickClient.h"

namespace LuaHookNG {
	class ILinkHandler;
	class LuaAppLink {
		std::unique_ptr<ILinkHandler> handler;

	public:
		DLLSPEC LuaAppLink(INewStateClient&);
		DLLSPEC LuaAppLink(ICloseStateClient&);
		DLLSPEC LuaAppLink(IOnTickClient&);
		DLLSPEC LuaAppLink(IOnRequireClient&);
		DLLSPEC LuaAppLink(LuaAppLink&&);
		DLLSPEC LuaAppLink& operator=(LuaAppLink&&);
		DLLSPEC ~LuaAppLink();

		template<typename Client>
		static std::vector<LuaAppLink> RegisterMultiple(Client& client) {
			constexpr bool isNewState =
			  std::is_base_of_v<INewStateClient, Client>;
			constexpr bool isCloseState =
			  std::is_base_of_v<ICloseStateClient, Client>;
			constexpr bool isOnTick = std::is_base_of_v<IOnTickClient, Client>;
			constexpr bool isOnRequire =
			  std::is_base_of_v<IOnRequireClient, Client>;
			static_assert(isNewState || isCloseState || isOnTick || isOnRequire,
						  "Your class must implement one of the available "
						  "client interfaces");
			std::vector<LuaAppLink> ret;
			if constexpr (isNewState)
				ret.emplace_back(static_cast<INewStateClient&>(client));
			if constexpr (isCloseState)
				ret.emplace_back(static_cast<ICloseStateClient&>(client));
			if constexpr (isOnTick)
				ret.emplace_back(static_cast<IOnTickClient&>(client));
			if constexpr (isOnRequire)
				ret.emplace_back(static_cast<IOnRequireClient&>(client));
			return ret;
		}
	};
}  // namespace LuaHookNG
