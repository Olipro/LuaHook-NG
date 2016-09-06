#pragma once

namespace Olipro {
	class PipePair {
		HANDLE r, w;
	public:
		PipePair();
		PipePair(PipePair&&);
		PipePair(const PipePair&) = delete;
		~PipePair();
		HANDLE read();
		HANDLE write();
	};
}
