#pragma once

#include <glad/glad.h>

class Texture
{
public:
	Texture(int width, int height);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void SetupCurrentTexture(int width, int height);
	void Bind(unsigned int slot) const;
	void Unbind() const;

	const GLuint& GetID() const;
	const int& GetWidth() const;
	const int& GetHeight() const;

private:
	GLuint m_TextureID;
	int m_Width;
	int m_Height;
};