#include "Framebuffer.h"
#include "GLLog.h"
#include <iostream>

Framebuffer::Framebuffer(const Window& window) : m_Window(window)
{
	OPENGL_CALL(glGenFramebuffers(1, &m_ID));
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));


	OPENGL_CALL(glGenTextures(1, &m_TextureID));
	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));

	OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	OPENGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0));


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: Framebuffer is not complete!" << std::endl;
	}
}

Framebuffer::~Framebuffer()
{
	OPENGL_CALL(glDeleteFramebuffers(1, &m_ID));
	OPENGL_CALL(glDeleteTextures(1, &m_TextureID));
}

void Framebuffer::Bind() const
{
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
}

void Framebuffer::Unbind() const
{
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::BindTexture() const
{
	OPENGL_CALL(glActiveTexture(GL_TEXTURE0));
	OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}