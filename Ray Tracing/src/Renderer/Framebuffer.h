#pragma once

#include <glad/glad.h>
#include "Window.h"

class Framebuffer
{
public:
	Framebuffer(const Window& window);
	~Framebuffer();

	void Bind() const;
	void Unbind() const;
	void BindTexture() const;

private:
	const Window& m_Window;
	GLuint m_ID;
	GLuint m_TextureID;
};