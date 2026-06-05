#pragma once

#include <memory>
#include "Core/Config.h"
#include "Scene/Camera/Camera.h"
#include "RenderContext.h"
#include "Renderer/RenderUtils/BVHBuilder.h"
#include "Renderer/Abstractions/Shader.h"
#include "Renderer/Abstractions/Framebuffer.h"
#include "Renderer/Abstractions/VertexArray.h"
#include "Renderer/Abstractions/VertexBuffer.h"

class BVHDebugPass
{
public:
	BVHDebugPass();

	void Render(const RenderContext& context, const BVHDebugConfig& config);
	void Reload();

private:
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<Shader> m_Shader;

	int m_LeafLineCount = 0;
	int m_InternalLineCount = 0;
	int m_RebuildCounter = 0;

	void SetData(const std::vector<BVHNode>& nodes);
};