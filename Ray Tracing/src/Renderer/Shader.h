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

	void SetFloat(const std::string& name, float v);
	void SetVec2(const std::string& name, const glm::vec2& vec);
	void SetVec3(const std::string& name, const glm::vec3& vec);
	void SetVec4(const std::string& name, const glm::vec4& vec);
	void SetUInt32(const std::string& name, uint32_t v);
	void SetMat3x3(const std::string& name, const glm::mat3x3& mat);
	void SetMat4x4(const std::string& name, const glm::mat4x4& mat);

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