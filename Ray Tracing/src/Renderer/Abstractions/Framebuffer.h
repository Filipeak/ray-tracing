#pragma once

#include <glad/glad.h>
#include "Texture.h"

class Framebuffer
{
public:
	Framebuffer(const Texture* texture);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;

	void Bind(unsigned int width, unsigned int height) const;
	void Unbind() const;
	void SetTexture(const Texture* texture) const;

	static void BindDefault(unsigned int width, unsigned int height);

private:
	GLuint m_ID;
};