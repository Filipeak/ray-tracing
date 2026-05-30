#pragma once

#include <memory>
#include <vector>
#include "Framebuffer.h"
#include "QuadRenderer.h"
#include "Shader.h"

/**
 * REF: https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
 * REF: https://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare/
 * REF: https://www.youtube.com/watch?v=tI70-HIc5ro
 * REF: https://www.youtube.com/watch?v=ml-5OGZC7vE
 */
class Bloom
{
public:
	Bloom(int windowWidth, int windowHeight, int bloomTextureSlot, size_t mipChainLength, std::shared_ptr<QuadRenderer> quad);
	~Bloom();

	void Render(int srcTextureId, float filterRadius);
	void Reload();
	void Rescale(int windowWidth, int windowHeight);
	void ChangeMipChainLength(size_t newMipChainLength);

private:
	int m_WindowWidth;
	int m_WindowHeight;

	int m_BloomTextureSlot;
	size_t m_MipChainLength;
	std::vector<std::unique_ptr<Texture>> m_BloomTextures;

	std::shared_ptr<QuadRenderer> m_Quad;
	std::unique_ptr<Shader> m_DownsamplingShader;
	std::unique_ptr<Shader> m_UpsamplingShader;
	std::unique_ptr<Framebuffer> m_Framebuffer;

	void RenderDownsamples(int srcTextureId);
	void RenderUpsamples(float filterRadius);
	void RenderTexture(const Texture* texture);
	void ClearTextures();
	void RecreateTexture();
};