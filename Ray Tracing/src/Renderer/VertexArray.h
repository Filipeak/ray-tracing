#pragma once

#include <glad/glad.h>
#include <vector>

struct BufferLayoutData
{
	GLuint count;
	GLuint size;
	GLuint type;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void PushLayoutFloat(unsigned int count);
	void SaveLayout();

private:
	GLuint m_ID;
	std::vector<BufferLayoutData> m_Layout;
	GLuint m_LayoutStride;
};