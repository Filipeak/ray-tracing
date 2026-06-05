#pragma once

#include <memory>
#include <vector>
#include "Core/Config.h"
#include "RenderContext.h"
#include "Renderer/Abstractions/Framebuffer.h"
#include "Renderer/Abstractions/Shader.h"
#include "Renderer/Abstractions/Texture.h"
#include "Renderer/RenderUtils/QuadRenderer.h"

/**
 * REF: https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
 * REF: https://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare/
 * REF: https://www.youtube.com/watch?v=tI70-HIc5ro
 * REF: https://www.youtube.com/watch?v=ml-5OGZC7vE
 */
class BloomPass
{
public:
	BloomPass(const QuadRenderer* quad);
	~BloomPass();

	void Render(const RenderContext& context, const BloomConfig& config, const Texture* mainTexture);
	void Reload();

	const Texture* GetBloomTexture() const { return m_BloomTextures.empty() ? nullptr : m_BloomTextures[0].get(); }

private:
	// General parameters
	int m_WindowWidth;
	int m_WindowHeight;

	// Mip chain parameters
	size_t m_MipChainLength;
	std::vector<std::unique_ptr<Texture>> m_BloomTextures;

	// Rendering resources
	const QuadRenderer* m_Quad;
	std::unique_ptr<Shader> m_DownsamplingShader;
	std::unique_ptr<Shader> m_UpsamplingShader;
	std::unique_ptr<Framebuffer> m_Framebuffer;

	// Utils functions
	void Resize(int windowWidth, int windowHeight);
	void ChangeMipChainLength(size_t newMipChainLength);

	// Rendering steps
	void RenderDownsamples();
	void RenderUpsamples(float filterRadius);
	void RenderTexture(const Texture* texture);

	// Textures management
	void ClearTextures();
	void RecreateTexture();
};