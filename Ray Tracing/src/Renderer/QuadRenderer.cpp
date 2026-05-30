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

	m_VertexArray = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices) / sizeof(float));
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

	m_VertexArray->PushLayoutFloat(3);
	m_VertexArray->PushLayoutFloat(2);
	m_VertexArray->SaveLayout();
}

void QuadRenderer::Draw() const
{
	m_VertexArray->Bind();

	OPENGL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}