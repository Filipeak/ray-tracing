#pragma once

#include <glad/glad.h>

class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer(const void* data, size_t length, size_t size);
	~ShaderStorageBuffer();

	ShaderStorageBuffer(const ShaderStorageBuffer& other) = delete;
	ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;

	void Bind(unsigned int bindingPoint) const;
	void Unbind() const;

private:
	GLuint m_ID;
};