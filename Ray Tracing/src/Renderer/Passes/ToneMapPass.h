#pragma once

#include <memory>
#include "Core/Config.h"
#include "RenderContext.h"
#include "Renderer/Abstractions/Shader.h"
#include "Renderer/Abstractions/Texture.h"
#include "Renderer/RenderUtils/QuadRenderer.h"

class ToneMapPass
{
public:
	ToneMapPass(const QuadRenderer* quad);

	void Render(const RenderContext& context, const BloomConfig& bloomConfig, const ToneMapConfig& config, const Texture* mainTexture, const Texture* bloomTexture);
	void Reload();

private:
	std::unique_ptr<Shader> m_ScreenShader;
	const QuadRenderer* m_Quad;
};