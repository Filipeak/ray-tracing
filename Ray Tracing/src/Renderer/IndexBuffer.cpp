#include "IndexBuffer.h"
#include "GLLog.h"

IndexBuffer::IndexBuffer(const unsigned int* buffer, size_t count)
{
	static_assert(sizeof(unsigned int) == sizeof(GLuint));

	OPENGL_CALL(glGenBuffers(1, &m_ID));
	OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), buffer, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	OPENGL_CALL(glDeleteBuffers(1, &m_ID));
}

void IndexBuffer::Bind() const
{
	OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IndexBuffer::Unbind() const
{
	OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}