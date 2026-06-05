#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Core/Config.h"
#include "RenderContext.h"
#include "Renderer/Abstractions/Shader.h"
#include "Renderer/Abstractions/Framebuffer.h"
#include "Renderer/Abstractions/Texture.h"
#include "Renderer/RenderUtils/QuadRenderer.h"

struct AccumTarget
{
	std::unique_ptr<Framebuffer> framebuffer;
	std::unique_ptr<Texture> texture;

	void Build(int w, int h)
	{
		texture = std::make_unique<Texture>(w, h);
		framebuffer = std::make_unique<Framebuffer>(texture.get());
	}
};

class RayTracePass
{
public:
	RayTracePass(const QuadRenderer* quad);

	void Render(const RenderContext& context, const RayTracingConfig& config);
	void Reload();

	const Texture* GetCurrentAccumTexture() const { return m_CurrentAccumTexture; }

private:
	// Main rendering resources
	std::unique_ptr<Shader> m_Shader;
	const QuadRenderer* m_Quad;

	// Accumulation targets for ping-pong rendering
	AccumTarget m_AccumTarget1;
	AccumTarget m_AccumTarget2;
	glm::mat4x4 m_LastCameraInverseProjection{ 0.0f };
	glm::mat4x4 m_LastCameraInverseView{ 0.0f };
	bool m_PingPongBufferSelect;
	const Texture* m_CurrentAccumTexture;

	// Helper state
	int m_AccumulationPasses;
	unsigned int m_FrameIndex;

	// Utils functions
	void Resize(int width, int height);
	void ResetAccumulation() { m_AccumulationPasses = 0; }
};