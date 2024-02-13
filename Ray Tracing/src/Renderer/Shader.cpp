#include "Shader.h"
#include "GLLog.h"
#include <iostream>
#include <fstream>

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	m_VertexShaderPath = vertexShaderPath;
	m_FragmentShaderPath = fragmentShaderPath;

	LinkShaderProgram();
}

Shader::~Shader()
{
	DeleteShaderProgram();
}

void Shader::Reload()
{
	m_UniformsMap.clear();

	DeleteShaderProgram();
	LinkShaderProgram();
	Bind();

	std::cout << "Reloaded shader!" << std::endl;
}

void Shader::Bind()
{
	OPENGL_CALL(glUseProgram(m_ID));
}

void Shader::Unbind()
{
	OPENGL_CALL(glUseProgram(0));
}

void Shader::SetUniform1f(std::string name, float v1)
{
	OPENGL_CALL(glUniform1f(GetUniformLocation(name), v1));
}

void Shader::SetUniform2f(std::string name, float v1, float v2)
{
	OPENGL_CALL(glUniform2f(GetUniformLocation(name), v1, v2));
}

void Shader::SetUniform3f(std::string name, float v1, float v2, float v3)
{
	OPENGL_CALL(glUniform3f(GetUniformLocation(name), v1, v2, v3));
}

void Shader::SetUniform4f(std::string name, float v1, float v2, float v3, float v4)
{
	OPENGL_CALL(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4));
}

void Shader::LinkShaderProgram()
{
	GLuint vertexShader = CompileShader(m_VertexShaderPath, true);
	GLuint fragmentShader = CompileShader(m_FragmentShaderPath, false);

	OPENGL_CALL(m_ID = glCreateProgram());

	OPENGL_CALL(glAttachShader(m_ID, vertexShader));
	OPENGL_CALL(glAttachShader(m_ID, fragmentShader));

	OPENGL_CALL(glLinkProgram(m_ID));
	OPENGL_CALL(glValidateProgram(m_ID));

	GLint result;
	OPENGL_CALL(glGetProgramiv(m_ID, GL_LINK_STATUS, &result));

	if (result == GL_FALSE)
	{
		char message[512] = {};
		OPENGL_CALL(glGetProgramInfoLog(m_ID, 512, NULL, (GLchar*)message));

		std::cout << "ERROR: Failed to link shader program!" << std::endl;
		std::cout << message;

		return;
	}

	OPENGL_CALL(glDetachShader(m_ID, vertexShader));
	OPENGL_CALL(glDetachShader(m_ID, fragmentShader));

	OPENGL_CALL(glDeleteShader(vertexShader));
	OPENGL_CALL(glDeleteShader(fragmentShader));
}

GLuint Shader::CompileShader(std::string path, bool isVertex)
{
	std::string content = GetFileContent(path);
	const char* source = content.c_str();

	OPENGL_CALL(GLuint shader = glCreateShader(isVertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));

	OPENGL_CALL(glShaderSource(shader, 1, &source, NULL));
	OPENGL_CALL(glCompileShader(shader));

	GLint result;
	OPENGL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		char message[512] = {};
		OPENGL_CALL(glGetShaderInfoLog(shader, 512, NULL, (GLchar*)message));

		std::cout << "ERROR: Failed to compile " << (isVertex ? "Vertex" : "Fragment") << " Shader!" << std::endl;
		std::cout << message;

		OPENGL_CALL(glDeleteShader(shader));

		return 0;
	}

	return shader;
}

void Shader::DeleteShaderProgram()
{
	OPENGL_CALL(glDeleteProgram(m_ID));
}

std::string Shader::GetFileContent(std::string path)
{
	std::ifstream file(path);

	if (file.good())
	{
		std::string line;
		std::string content;

		while (std::getline(file, line))
		{
			content += line + "\n";
		}

		file.close();

		return content;
	}
	else
	{
		std::cout << "ERROR: Couldn't find shader file: " << path << std::endl;

		return "";
	}
}

GLint Shader::GetUniformLocation(std::string name)
{
	if (m_UniformsMap.find(name) != m_UniformsMap.end())
	{
		return m_UniformsMap[name];
	}
	else
	{
		OPENGL_CALL(GLint location = glGetUniformLocation(m_ID, name.c_str()));

		if (location == -1)
		{
			std::cout << "ERROR: Uniform invalid location: " << name << std::endl;

			return -1;
		}

		m_UniformsMap[name] = location;
		
		return location;
	}
}