#include "Bloom.h"
#include "GLLog.h"
#include <iostream>

Bloom::Bloom(int windowWidth, int windowHeight, int bloomTextureSlot, size_t mipChainLength, std::shared_ptr<QuadRenderer> quad) : m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), m_BloomTextureSlot(bloomTextureSlot), m_MipChainLength(mipChainLength), m_Quad(quad)
{
	m_DownsamplingShader = std::make_unique<Shader>("shaders/quad.vert", "shaders/bloom_downsample.frag");
	m_UpsamplingShader = std::make_unique<Shader>("shaders/quad.vert", "shaders/bloom_upsample.frag");

	RecreateTexture();

	m_Framebuffer = std::make_unique<Framebuffer>(m_BloomTextures[0].get());
}

Bloom::~Bloom()
{
	ClearTextures();
}

void Bloom::Render(int srcTextureId, float filterRadius)
{
	Texture::SelectSlot(m_BloomTextureSlot);

	m_Framebuffer->Bind();

	RenderDownsamples(srcTextureId);
	RenderUpsamples(filterRadius);

	m_Framebuffer->Unbind();
	
	// Bind the first bloom texture for use in the next shader (e.g. screen shader)
	m_BloomTextures[0]->Bind();
}

void Bloom::Reload()
{
	m_DownsamplingShader->Reload();
	m_UpsamplingShader->Reload();

	std::cout << "Bloom shaders were reloaded" << std::endl;
}

void Bloom::Rescale(int windowWidth, int windowHeight)
{
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	ClearTextures();
	RecreateTexture();
}

void Bloom::ChangeMipChainLength(size_t newMipChainLength)
{
	m_MipChainLength = newMipChainLength;

	ClearTextures();
	RecreateTexture();
}

void Bloom::RenderDownsamples(int srcTextureId)
{
	m_DownsamplingShader->Bind();

	int currentTexture = srcTextureId;

	for (size_t i = 0; i < m_BloomTextures.size(); i++)
	{
		const Texture* tex = m_BloomTextures[i].get();

		m_DownsamplingShader->SetInt("u_ScreenTexture", currentTexture);
		m_DownsamplingShader->SetVec2("u_Resolution", glm::vec2(tex->GetWidth(), tex->GetHeight()));

		RenderTexture(tex);

		tex->Bind(); // Current slot is already set to bloom texture slot
		currentTexture = m_BloomTextureSlot;
	}
}

void Bloom::RenderUpsamples(float filterRadius)
{
	m_UpsamplingShader->Bind();

	OPENGL_CALL(glEnable(GL_BLEND));
	OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE));
	OPENGL_CALL(glBlendEquation(GL_FUNC_ADD));

	for (size_t i = m_BloomTextures.size() - 1; i > 0; i--)
	{
		const Texture* tex = m_BloomTextures[i].get();
		const Texture* nextTex = m_BloomTextures[i - 1].get();

		tex->Bind();

		m_UpsamplingShader->SetInt("u_ScreenTexture", m_BloomTextureSlot);
		m_UpsamplingShader->SetFloat("u_FilterRadius", filterRadius);

		RenderTexture(nextTex);
	}

	OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	OPENGL_CALL(glDisable(GL_BLEND));
}

void Bloom::RenderTexture(const Texture* texture)
{
	Framebuffer::SetGlobalViewportSize(texture->GetWidth(), texture->GetHeight());

	m_Framebuffer->SetTexture(texture);
	m_Quad->Draw();
}

void Bloom::ClearTextures()
{
	m_BloomTextures.clear();

	std::cout << "Bloom textures were cleared" << std::endl;
}

void Bloom::RecreateTexture()
{
	Texture::SelectSlot(m_BloomTextureSlot);

	m_BloomTextures.reserve(m_MipChainLength);

	int texWidth = m_WindowWidth;
	int texHeight = m_WindowHeight;

	for (size_t i = 0; i < m_MipChainLength; i++)
	{
		texWidth /= 2;
		texHeight /= 2;

		m_BloomTextures.emplace_back(std::make_unique<Texture>(texWidth, texHeight));
	}

	std::cout << "Bloom textures were recreated with new size: " << m_WindowWidth << "x" << m_WindowHeight << " and mip chain length: " << m_MipChainLength << std::endl;
}