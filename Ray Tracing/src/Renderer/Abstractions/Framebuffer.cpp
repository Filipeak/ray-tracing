#include "Framebuffer.h"
#include "GLLog.h"
#include "Core/EngineAssert.h"

Framebuffer::Framebuffer(const Texture* texture)
{
	OPENGL_CALL(glGenFramebuffers(1, &m_ID));

	if (texture != nullptr)
	{
		Bind(0, 0); // Viewport doesn't matter since we only set the texture
		SetTexture(texture);
		Unbind();
	}
}

Framebuffer::~Framebuffer()
{
	if (m_ID != 0)
	{
		OPENGL_CALL(glDeleteFramebuffers(1, &m_ID));
	}
}

void Framebuffer::Bind(unsigned int width, unsigned int height) const
{
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
	OPENGL_CALL(glViewport(0, 0, width, height));
}

void Framebuffer::Unbind() const
{
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::SetTexture(const Texture* texture) const
{
	OPENGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0));
	
	ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
}

void Framebuffer::BindDefault(unsigned int width, unsigned int height)
{
	OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	OPENGL_CALL(glViewport(0, 0, width, height));
}