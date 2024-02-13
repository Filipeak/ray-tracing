#include "Framebuffer.h"
#include "GLLog.h"

Framebuffer::Framebuffer()
{
	OPENGL_CALL(glGenFramebuffers(1, &m_ID));
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