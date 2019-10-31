#pragma once

#include <shared_mutex>
#include <vector>

namespace LuaHookNG {
	template<typename ClientT>
	class RegistrationManager {
		std::vector<ClientT*> clients;
		std::shared_mutex mutex;

	public:
		void AddClient(ClientT* client) {
			std::unique_lock l{mutex};
			clients.emplace_back(client);
		}

		template<typename Callable>
		void CallClients(Callable callable) {
			std::shared_lock l{mutex};
			for (auto&& client : clients)
				callable(client);
		}

		void DelClient(ClientT* client) {
			std::unique_lock l{mutex};
			clients.erase(std::find(clients.begin(), clients.end(), client));
		}
	};
}  // namespace LuaHookNG
