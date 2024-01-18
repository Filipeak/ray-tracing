#pragma once

#include <cassert>

#define OPENGL_CALL(x) OpenGLLog::ClearError(); x; assert(OpenGLLog::CheckError(#x, __FILE__, __LINE__))

class OpenGLLog
{
public:
	static void ClearError();
	static bool CheckError(const char* function, const char* file, int line);
};