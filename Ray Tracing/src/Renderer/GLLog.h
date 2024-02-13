#pragma once

#include <cassert>

#define OPENGL_CALL(x) OpenGLLog::ClearError(); x; if (!OpenGLLog::CheckError(#x, __FILE__, __LINE__)) { assert(false); }

class OpenGLLog
{
public:
	static void ClearError();
	static bool CheckError(const char* function, const char* file, int line);
};