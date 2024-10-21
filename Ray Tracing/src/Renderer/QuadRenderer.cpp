#include "QuadRenderer.h"
#include "GLLog.h"

QuadRenderer::QuadRenderer()
{
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		+1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, +1.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	m_VertexArray = new VertexArray();
	m_VertexBuffer = new VertexBuffer(vertices, sizeof(vertices) / sizeof(float));
	m_IndexBuffer = new IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

	m_VertexArray->PushLayoutFloat(3);
	m_VertexArray->PushLayoutFloat(2);
	m_VertexArray->SaveLayout();
}

QuadRenderer::~QuadRenderer()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_IndexBuffer;
}

void QuadRenderer::Draw() const
{
	m_VertexArray->Bind();

	OPENGL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}