#pragma once

#include <memory>
#include "Window.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class QuadRenderer
{
public:
	QuadRenderer();

	void Draw() const;

private:
	std::unique_ptr<VertexArray> m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
};