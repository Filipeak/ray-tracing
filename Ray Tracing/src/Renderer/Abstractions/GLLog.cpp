#include <glad/glad.h>
#include "GLLog.h"
#include "Core/Logger.h"
#include <cstdlib>

void OpenGLLog::ClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool OpenGLLog::CheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		spdlog::error("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line);

		return false;
	}

	return true;
}

void OpenGLLog::FailureHandler()
{
	abort();
}