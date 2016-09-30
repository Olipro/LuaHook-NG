#pragma once
#include "PipePair.h"

namespace Olipro {
	class ConsoleManager {
		PipePair stdOut, stdErr, stdIn;
		std::ifstream usrCin;
		std::ofstream usrCout, usrCerr;
		std::wifstream usrWcin;
		std::wofstream usrWcout, usrWcerr;
		HANDLE realStdOut, realStdErr, realStdIn;
		std::atomic_char skipCounter;
		class AutoJoinThread {
			std::thread t;
		public:
			AutoJoinThread() = default;
			AutoJoinThread(std::thread&& thread) : t(std::move(thread)) {}
			~AutoJoinThread() { if (t.joinable()) t.join(); }
			AutoJoinThread& operator=(AutoJoinThread&&) = default;
		} inThread, outThread, errThread;

		void ProcessPipe(HANDLE, HANDLE, std::ofstream* const, bool);
		void TextWasDisplayed();
	public:
		ConsoleManager();
		void Run();
	};
}
