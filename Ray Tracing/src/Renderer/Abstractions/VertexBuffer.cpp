#include "VertexBuffer.h"
#include "GLLog.h"

VertexBuffer::VertexBuffer(const float* buffer, size_t count)
{
	static_assert(sizeof(float) == sizeof(GLfloat));

	OPENGL_CALL(glGenBuffers(1, &m_ID));
	
	Bind();

	OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), buffer, GL_STATIC_DRAW));
	
	Unbind();
}

VertexBuffer::~VertexBuffer()
{
	if (m_ID != 0)
	{
		OPENGL_CALL(glDeleteBuffers(1, &m_ID));
	}
}

void VertexBuffer::Bind() const
{
	OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

void VertexBuffer::Unbind() const
{
	OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}