#include "RayTracePass.h"
#include "Core/Logger.h"

RayTracePass::RayTracePass(const QuadRenderer* quad) : m_Quad(quad), m_PingPongBufferSelect(true), m_CurrentAccumTexture(nullptr), m_AccumulationPasses(0), m_FrameIndex(1)
{
	m_Shader = std::make_unique<Shader>("shaders/quad.vert", "shaders/raytracing.frag");
}

void RayTracePass::Render(const RenderContext& context, const RayTracingConfig& config)
{
	// Ping-pong accumulation target selection
	const AccumTarget& readTarget = m_PingPongBufferSelect ? m_AccumTarget1 : m_AccumTarget2;
	const AccumTarget& writeTarget = m_PingPongBufferSelect ? m_AccumTarget2 : m_AccumTarget1;

	// Resize accumulation targets if viewport size changed
	if (readTarget.framebuffer == nullptr || context.viewport.width != readTarget.texture->GetWidth() || context.viewport.height != readTarget.texture->GetHeight())
	{
		Resize(context.viewport.width, context.viewport.height);
	}

	// Bind
	readTarget.texture->Bind(0);
	writeTarget.framebuffer->Bind(context.viewport.width, context.viewport.height);

	context.gpuScene.Bind();
	m_Shader->Bind();

	// Calculate camera matrices
	const auto& cameraInverseProjection = context.camera.GetInverseProjectionMatrix();
	const auto& cameraInverseView = context.camera.GetInverseViewMatrix();

	// Reset accumulation if camera moved
	if (cameraInverseProjection != m_LastCameraInverseProjection || cameraInverseView != m_LastCameraInverseView)
	{
		m_LastCameraInverseProjection = cameraInverseProjection;
		m_LastCameraInverseView = cameraInverseView;

		ResetAccumulation();
	}
	
	// Draw
	m_Shader->SetInt("u_ScreenTexture", 0);
	m_Shader->SetUInt("u_AccumulationPasses", m_AccumulationPasses);
	m_Shader->SetVec2("u_Resolution", glm::vec2(context.viewport.width, context.viewport.height));
	m_Shader->SetUInt("u_FrameIndex", m_FrameIndex);
	m_Shader->SetVec3("u_CameraPosition", context.camera.GetPosition());
	m_Shader->SetMat4x4("u_CameraInverseProjection", cameraInverseProjection);
	m_Shader->SetMat4x4("u_CameraInverseView", cameraInverseView);
	m_Quad->Draw();

	// Unbind
	context.gpuScene.Unbind();
	m_Shader->Unbind();

	readTarget.texture->Unbind();
	writeTarget.framebuffer->Unbind();

	// Forward accumulation results
	m_CurrentAccumTexture = writeTarget.texture.get();

	// Update state
	m_PingPongBufferSelect = !m_PingPongBufferSelect;
	m_AccumulationPasses = config.accumulate ? m_AccumulationPasses + 1 : 0;
	m_FrameIndex++;
}

void RayTracePass::Reload()
{
	m_Shader->Reload();

	ResetAccumulation();
}

void RayTracePass::Resize(int width, int height)
{
	m_AccumTarget1.Build(width, height);
	m_AccumTarget2.Build(width, height);

	ResetAccumulation();

	spdlog::debug("Resized accumulation targets to {}x{}", width, height);
}