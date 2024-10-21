#include "VertexArray.h"
#include "GLLog.h"

VertexArray::VertexArray()
{
	OPENGL_CALL(glGenVertexArrays(1, &m_ID));
	OPENGL_CALL(glBindVertexArray(m_ID));

	m_LayoutStride = 0;
}

VertexArray::~VertexArray()
{
	OPENGL_CALL(glDeleteVertexArrays(1, &m_ID));
}

void VertexArray::Bind() const
{
	OPENGL_CALL(glBindVertexArray(m_ID));
}

void VertexArray::Unbind() const
{
	OPENGL_CALL(glBindVertexArray(0));
}

void VertexArray::PushLayoutFloat(unsigned int count)
{
	static_assert(sizeof(unsigned int) == sizeof(GLuint));

	BufferLayoutData layoutData = { count, count * sizeof(float), GL_FLOAT };

	m_Layout.push_back(layoutData);
	m_LayoutStride += layoutData.size;
}

void VertexArray::SaveLayout()
{
	size_t offset = 0;

	for (GLuint i = 0; i < m_Layout.size(); i++)
	{
		OPENGL_CALL(glVertexAttribPointer(i, m_Layout[i].count, m_Layout[i].type, GL_FALSE, m_LayoutStride, (const void*)offset));
		OPENGL_CALL(glEnableVertexAttribArray(i));

		offset += m_Layout[i].size;
	}
}