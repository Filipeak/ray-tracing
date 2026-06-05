#include "BVHDebugPass.h"
#include "Renderer/Abstractions/GLLog.h"

BVHDebugPass::BVHDebugPass()
{
	m_Shader = std::make_unique<Shader>("shaders/debug.vert", "shaders/debug.frag");
}

void BVHDebugPass::Render(const RenderContext& context, const BVHDebugConfig& config)
{
	if (!config.enabled)
	{
		return;
	}

	if (context.gpuScene.GetRebuildCounter() != m_RebuildCounter)
	{
		SetData(context.gpuScene.GetBVHBuilder().GetNodes());

		m_RebuildCounter = context.gpuScene.GetRebuildCounter();
	}

	glm::mat4 vp = context.camera.GetProjectionMatrix() * context.camera.GetViewMatrix();

	OPENGL_CALL(glEnable(GL_DEPTH_TEST));
	OPENGL_CALL(glDepthFunc(GL_ALWAYS));

	Framebuffer::BindDefault(context.viewport.width, context.viewport.height);

	m_Shader->Bind();
	m_Shader->SetMat4x4("u_VP", vp);
	m_VAO->Bind();

	// Cyan for leaf nodes
	m_Shader->SetVec3("u_Color", glm::vec3(0.0f, 1.0f, 1.0f));
	OPENGL_CALL(glDrawArrays(GL_LINES, 0, m_LeafLineCount));

	// Yellow for internal nodes
	m_Shader->SetVec3("u_Color", glm::vec3(1.0f, 1.0f, 0.0f));
	OPENGL_CALL(glDrawArrays(GL_LINES, m_LeafLineCount, m_InternalLineCount));

	m_Shader->Unbind();
	m_VAO->Unbind();

	OPENGL_CALL(glDepthFunc(GL_LESS));
	OPENGL_CALL(glDisable(GL_DEPTH_TEST));
}

void BVHDebugPass::Reload()
{
	m_Shader->Reload();
}

void BVHDebugPass::SetData(const std::vector<BVHNode>& nodes)
{
	// Build debug line geometry: cyan for leaf nodes, yellow for internal nodes
	auto pushEdges = [](std::vector<float>& buf, const glm::vec3& mn, const glm::vec3& mx)
		{
			auto push = [&](glm::vec3 a, glm::vec3 b) { buf.insert(buf.end(), { a.x, a.y, a.z, b.x, b.y, b.z }); };

			push({ mn.x, mn.y, mn.z }, { mx.x, mn.y, mn.z });
			push({ mx.x, mn.y, mn.z }, { mx.x, mn.y, mx.z });
			push({ mx.x, mn.y, mx.z }, { mn.x, mn.y, mx.z });
			push({ mn.x, mn.y, mx.z }, { mn.x, mn.y, mn.z });
			push({ mn.x, mx.y, mn.z }, { mx.x, mx.y, mn.z });
			push({ mx.x, mx.y, mn.z }, { mx.x, mx.y, mx.z });
			push({ mx.x, mx.y, mx.z }, { mn.x, mx.y, mx.z });
			push({ mn.x, mx.y, mx.z }, { mn.x, mx.y, mn.z });
			push({ mn.x, mn.y, mn.z }, { mn.x, mx.y, mn.z });
			push({ mx.x, mn.y, mn.z }, { mx.x, mx.y, mn.z });
			push({ mx.x, mn.y, mx.z }, { mx.x, mx.y, mx.z });
			push({ mn.x, mn.y, mx.z }, { mn.x, mx.y, mx.z });
		};

	std::vector<float> leafLines, internalLines;

	for (const auto& node : nodes)
	{
		if (node.triCount > 0)
		{
			pushEdges(leafLines, node.bounds.minBounds, node.bounds.maxBounds);
		}
		else
		{
			pushEdges(internalLines, node.bounds.minBounds, node.bounds.maxBounds);
		}
	}

	// Concatenate: leaf lines first, then internal lines
	std::vector<float> allLines = leafLines;
	allLines.insert(allLines.end(), internalLines.begin(), internalLines.end());

	m_LeafLineCount = (int)(leafLines.size() / 3);
	m_InternalLineCount = (int)(internalLines.size() / 3);

	// Create GPU buffers
	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(allLines.data(), allLines.size());

	m_VAO->Bind();
	m_VBO->Bind();

	m_VAO->PushLayoutFloat(3);
	m_VAO->SaveLayout();

	m_VAO->Unbind();
	m_VBO->Unbind();
}