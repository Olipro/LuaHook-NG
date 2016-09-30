#pragma once
#include "ConsoleManager.h"

namespace Olipro {
	class DeveloperTools {
		static DeveloperTools self;
		ConsoleManager console;

		DeveloperTools();
	};
}
