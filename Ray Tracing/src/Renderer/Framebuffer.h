#pragma once

#include <glad/glad.h>
#include "Texture.h"

class Framebuffer
{
public:
	Framebuffer(const Texture* texture);
	~Framebuffer();

	void Bind() const;
	void Unbind() const;
	void SetTexture(const Texture* texture);

	static void SetViewportSize(unsigned int width, unsigned int height);

private:
	GLuint m_ID;
};