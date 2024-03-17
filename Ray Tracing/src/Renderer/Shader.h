#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
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

	void SetUniform1f(const std::string& name, float v1);
	void SetUniform2f(const std::string& name, float v1, float v2);
	void SetUniform3f(const std::string& name, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void SetUniform1ui(const std::string& name, uint32_t v1);
	void SetUniformVec3(const std::string& name, const glm::vec3& vec);
	void SetUniformMat3x3(const std::string& name, const glm::mat3x3& mat);
	void SetUniformMat4x4(const std::string& name, const glm::mat4x4& mat);

private:
	GLuint m_ID;
	std::string m_VertexShaderPath;
	std::string m_FragmentShaderPath;
	std::unordered_map<std::string, GLint> m_UniformsMap;

	void LinkShaderProgram();
	GLuint CompileShader(std::string path, bool isVertex);
	void DeleteShaderProgram();
	std::string GetFileContent(std::string path);
	GLint GetUniformLocation(const std::string& name);
};