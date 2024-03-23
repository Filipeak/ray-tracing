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

void Shader::SetFloat(const std::string& name, float v)
{
	OPENGL_CALL(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetVec2(const std::string& name, const glm::vec2& vec)
{
	OPENGL_CALL(glUniform2f(GetUniformLocation(name), vec.x, vec.y));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vec)
{
	OPENGL_CALL(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& vec)
{
	OPENGL_CALL(glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
}

void Shader::SetUInt32(const std::string& name, uint32_t v)
{
	OPENGL_CALL(glUniform1ui(GetUniformLocation(name), v));
}

void Shader::SetMat3x3(const std::string& name, const glm::mat3x3& mat)
{
	OPENGL_CALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetMat4x4(const std::string& name, const glm::mat4x4& mat)
{
	OPENGL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
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

GLint Shader::GetUniformLocation(const std::string& name)
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