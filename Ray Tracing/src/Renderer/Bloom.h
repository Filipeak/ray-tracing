#pragma once

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
	Bloom(int windowWidth, int windowHeight, size_t mipChainLength, const QuadRenderer* quad);
	~Bloom();

	void Render(int srcTextureId, int bloomTextureId, float filterRadius);
	void Reload();
	void Rescale(int windowWidth, int windowHeight);
	void ChangeMipChainLength(size_t newMipChainLength);

private:
	int m_WindowWidth;
	int m_WindowHeight;
	size_t m_MipChainLength;
	const QuadRenderer* m_Quad;
	Shader* m_DownsamplingShader;
	Shader* m_UpsamplingShader;
	std::vector<Texture*> m_BloomTextures;
	Framebuffer* m_Framebuffer;

	void RenderDownsamples(int srcTextureId, int bloomTextureId);
	void RenderUpsamples(int bloomTextureId, float filterRadius);
	void ClearTextures();
	void RecreateTexture();
};