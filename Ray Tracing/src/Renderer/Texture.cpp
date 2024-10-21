#include "Texture.h"
#include "GLLog.h"

Texture::Texture(int width, int height)
{
	OPENGL_CALL(glGenTextures(1, &m_TextureID));

	SetupCurrentTexture(width, height);
}

Texture::~Texture()
{
	OPENGL_CALL(glDeleteTextures(1, &m_TextureID));
}

void Texture::Rescale(int width, int height)
{
	SetupCurrentTexture(width, height);
}

void Texture::Bind() const
{
	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture::Unbind() const
{
	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

const GLuint& Texture::GetID() const
{
	return m_TextureID;
}

const int& Texture::GetWidth() const
{
	return m_Width;
}

const int& Texture::GetHeight() const
{
	return m_Height;
}

void Texture::SelectSlot(unsigned int slot)
{
	OPENGL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
}

void Texture::SetupCurrentTexture(int width, int height)
{
	m_Width = width;
	m_Height = height;

	Bind();

	OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	Unbind();
}