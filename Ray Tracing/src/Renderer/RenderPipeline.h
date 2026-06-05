#pragma once

#include <memory>
#include "Core/Config.h"
#include "Passes/RenderContext.h"
#include "Passes/RayTracePass.h"
#include "Passes/BloomPass.h"
#include "Passes/ToneMapPass.h"
#include "Passes/BVHDebugPass.h"
#include "RenderUtils/QuadRenderer.h"

class RenderPipeline
{
public:
	RenderPipeline();

	void Execute(const RenderContext& context, const RayTracingConfig& rayTracingConfig, const BloomConfig& bloomConfig, const ToneMapConfig& toneMapConfig, const BVHDebugConfig& bvhDebugConfig);
	void Reload();

private:
	std::unique_ptr<QuadRenderer> m_Quad;

	std::unique_ptr<RayTracePass> m_RayTracePass;
	std::unique_ptr<BloomPass> m_BloomPass;
	std::unique_ptr<ToneMapPass> m_ToneMapPass;
	std::unique_ptr<BVHDebugPass> m_BVHDebugPass;
};