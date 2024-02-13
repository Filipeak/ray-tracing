#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
	~Shader();

	void Reload();
	void Bind();
	void Unbind();

	void SetUniform1f(std::string name, float v1);
	void SetUniform2f(std::string name, float v1, float v2);
	void SetUniform3f(std::string name, float v1, float v2, float v3);
	void SetUniform4f(std::string name, float v1, float v2, float v3, float v4);

private:
	GLuint m_ID;
	std::string m_VertexShaderPath;
	std::string m_FragmentShaderPath;
	std::unordered_map<std::string, GLint> m_UniformsMap;

	void LinkShaderProgram();
	GLuint CompileShader(std::string path, bool isVertex);
	void DeleteShaderProgram();
	std::string GetFileContent(std::string path);
	GLint GetUniformLocation(std::string name);
};