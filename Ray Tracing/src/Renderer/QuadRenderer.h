#pragma once

#include "Window.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class QuadRenderer
{
public:
	QuadRenderer();
	~QuadRenderer();

	void Draw() const;

private:
	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;
};