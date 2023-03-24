#include "log.h"

#include <iostream>

void Debug::log(const std::string& message, DebugLevel level)
{
#ifdef _DEBUG
	switch (level)
	{
		case Debug::WARNING:
			std::cout << "[WARNING]:  " << message;
			break;

		case Debug::ERROR:
			std::cout << "[ERROR]:  " << message;
			break;

		default:
			std::cout << "[INFO]:  " << message;
			break;
	}
#endif
}

void Debug::log(const char* message, DebugLevel level)
{
#ifdef _DEBUG
	Debug::log(std::string(message), level);
#endif
}
