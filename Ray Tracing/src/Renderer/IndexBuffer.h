#pragma once

#include <glad/glad.h>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* buffer, size_t count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;
};