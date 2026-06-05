#include "ShaderStorageBuffer.h"
#include "GLLog.h"

ShaderStorageBuffer::ShaderStorageBuffer(const void* data, size_t length, size_t size)
{
	OPENGL_CALL(glGenBuffers(1, &m_ID));
	OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID));
	OPENGL_CALL(glBufferData(GL_SHADER_STORAGE_BUFFER, length * size, data, GL_STATIC_DRAW));
	OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	if (m_ID != 0)
	{
		OPENGL_CALL(glDeleteBuffers(1, &m_ID));
	}
}

void ShaderStorageBuffer::Bind(unsigned int bindingPoint) const
{
	OPENGL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_ID));
}

void ShaderStorageBuffer::Unbind() const
{
	OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}