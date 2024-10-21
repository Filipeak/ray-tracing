#pragma once

#include <glad/glad.h>

class Texture
{
public:
	Texture(int width, int height);
	~Texture();

	void Rescale(int width, int height);
	void Bind() const;
	void Unbind() const;

	const GLuint& GetID() const;
	const int& GetWidth() const;
	const int& GetHeight() const;

	static void SelectSlot(unsigned int slot);

private:
	GLuint m_TextureID;
	int m_Width;
	int m_Height;

	void SetupCurrentTexture(int width, int height);
};