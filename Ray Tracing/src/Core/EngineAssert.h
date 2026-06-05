#pragma once

#define ENGINE_ASSERT(x, msg) do { if (!(x)) { engine_assert_handler(#x, __FILE__, __LINE__, msg); } } while (0)

void engine_assert_handler(const char* expression, const char* file, int line, const char* message);