#include "EngineAssert.h"
#include "Logger.h"
#include <cstdlib>

void engine_assert_handler(const char* expression, const char* file, int line, const char* message)
{
	spdlog::critical("Assertion Failed: {0} | {1}:{2} | Expression: {3}", message, file, line, expression);

	abort();
}