#include <glad/glad.h>
#include "RenderPipeline.h"
#include "Renderer/Abstractions/GLLog.h"
#include "Core/Logger.h"

RenderPipeline::RenderPipeline()
{
	m_Quad = std::make_unique<QuadRenderer>();

	m_RayTracePass = std::make_unique<RayTracePass>(m_Quad.get());
	m_BloomPass = std::make_unique<BloomPass>(m_Quad.get());
	m_ToneMapPass = std::make_unique<ToneMapPass>(m_Quad.get());
	m_BVHDebugPass = std::make_unique<BVHDebugPass>();

	spdlog::info("RenderPipeline initialized successfully.");
	spdlog::debug("OpenGL Info:");

	OPENGL_CALL(char* vendor = (char*)glGetString(GL_VENDOR));
	OPENGL_CALL(char* renderer = (char*)glGetString(GL_RENDERER));
	OPENGL_CALL(char* version = (char*)glGetString(GL_VERSION));

	spdlog::debug("> Vendor: {}", vendor);
	spdlog::debug("> Renderer: {}", renderer);
	spdlog::debug("> Version: {}", version);
}

void RenderPipeline::Execute(const RenderContext& context, const RayTracingConfig& rayTracingConfig, const BloomConfig& bloomConfig, const ToneMapConfig& toneMapConfig, const BVHDebugConfig& bvhDebugConfig)
{
	m_RayTracePass->Render(context, rayTracingConfig);
	m_BloomPass->Render(context, bloomConfig, m_RayTracePass->GetCurrentAccumTexture());
	m_ToneMapPass->Render(context, bloomConfig, toneMapConfig, m_RayTracePass->GetCurrentAccumTexture(), m_BloomPass->GetBloomTexture());
	m_BVHDebugPass->Render(context, bvhDebugConfig);
}

void RenderPipeline::Reload()
{
	m_RayTracePass->Reload();
	m_BloomPass->Reload();
	m_ToneMapPass->Reload();
	m_BVHDebugPass->Reload();

	spdlog::info("RenderPipeline shaders reloaded successfully.");
}