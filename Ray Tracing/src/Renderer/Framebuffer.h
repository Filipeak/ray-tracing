#pragma once

#include <glad/glad.h>

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;
};