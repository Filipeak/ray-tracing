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
	void Bind() const;
	void Unbind() const;

	void SetInt(const std::string& name, int v);
	void SetUInt(const std::string& name, unsigned int v);
	void SetFloat(const std::string& name, float v);
	void SetVec2(const std::string& name, const glm::vec2& vec);
	void SetVec3(const std::string& name, const glm::vec3& vec);
	void SetVec4(const std::string& name, const glm::vec4& vec);
	void SetMat4x4(const std::string& name, const glm::mat4x4& mat);

private:
	GLuint m_ID;
	std::string m_VertexShaderPath;
	std::string m_FragmentShaderPath;
	std::unordered_map<std::string, GLint> m_UniformsMap;

	void LinkShaderProgram();
	GLuint CompileShader(std::string path, bool isVertex) const;
	void DeleteShaderProgram() const;

	std::string GetFileContent(std::string path) const;
	GLint GetUniformLocation(const std::string& name);
};