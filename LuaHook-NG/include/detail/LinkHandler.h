#pragma once

#include <utility>

#include "detail/RegistrationManager.h"
#include "detail/interface/ILinkHandler.h"

namespace LuaHookNG {
	template<typename ClientT>
	class LinkHandler : public ILinkHandler {
		static inline RegistrationManager<ClientT> regManager;
		ClientT& client;

	public:
		LinkHandler(ClientT& client) : client{client} {
			regManager.AddClient(&client);
		}

		template<typename Callable>
		static void CallClients(Callable&& callable) noexcept {
			regManager.CallClients(std::forward<Callable>(callable));
		}

		~LinkHandler() {
			regManager.DelClient(&client);
		}
	};
}  // namespace LuaHookNG
