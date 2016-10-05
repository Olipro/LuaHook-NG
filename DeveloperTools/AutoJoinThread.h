#pragma once

namespace Olipro {
	class AutoJoinThread {
		std::thread t;
	public:
		AutoJoinThread() = default;
		AutoJoinThread(std::thread&& thread) : t(std::move(thread)) {}
		~AutoJoinThread() { if (t.joinable()) t.join(); }
		AutoJoinThread& operator=(AutoJoinThread&&) = default;
	};
}
