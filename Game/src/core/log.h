#pragma once

#include <string>

namespace Debug
{
	enum DebugLevel : int
	{
		INFO	= 0,
		WARNING = 1,
		ERROR	= 2,
	};

	void log(const std::string& message, DebugLevel level = INFO);
	void log(const char* message, DebugLevel level = INFO);
}