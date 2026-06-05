#include "Texture.h"
#include "GLLog.h"
#include <cstdio>

Texture::Texture(int width, int height)
{
	OPENGL_CALL(glGenTextures(1, &m_TextureID));

	Bind(0);
	SetupCurrentTexture(width, height);
	Unbind();
}

Texture::~Texture()
{
	if (m_TextureID != 0)
	{
		OPENGL_CALL(glDeleteTextures(1, &m_TextureID));
	}
}

void Texture::SetupCurrentTexture(int width, int height)
{
	m_Width = width;
	m_Height = height;

	OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

void Texture::Bind(unsigned int slot) const
{
	OPENGL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
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