#include "Framebuffer.h"
#include "GLLog.h"
#include <iostream>

Framebuffer::Framebuffer(const Texture* texture)
{
	OPENGL_CALL(glGenFramebuffers(1, &m_ID));
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));

	SetTexture(texture);
}

Framebuffer::~Framebuffer()
{
	OPENGL_CALL(glDeleteFramebuffers(1, &m_ID));
}

void Framebuffer::Bind() const
{
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
}

void Framebuffer::Unbind() const
{
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::SetTexture(const Texture* texture)
{
	OPENGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: Framebuffer is not complete!" << std::endl;
	}
}

void Framebuffer::SetViewportSize(unsigned int width, unsigned int height)
{
	OPENGL_CALL(glViewport(0, 0, width, height));
}