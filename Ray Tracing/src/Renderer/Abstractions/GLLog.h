#pragma once


#define OPENGL_CALL(x) OpenGLLog::ClearError(); x; if (!OpenGLLog::CheckError(#x, __FILE__, __LINE__)) { OpenGLLog::FailureHandler(); }

class OpenGLLog
{
public:
	static void ClearError();
	static bool CheckError(const char* function, const char* file, int line);
	static void FailureHandler();
};