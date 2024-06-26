#include "GLLog.h"
#include <glad/glad.h>
#include <iostream>

void OpenGLLog::ClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool OpenGLLog::CheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;

		return false;
	}

	return true;
}