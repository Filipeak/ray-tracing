#pragma once

#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer(const float* buffer, size_t count);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;
};