#pragma once

#include <memory>
#include "Renderer/Abstractions/VertexArray.h"
#include "Renderer/Abstractions/VertexBuffer.h"
#include "Renderer/Abstractions/IndexBuffer.h"

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