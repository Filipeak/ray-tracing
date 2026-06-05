#include <glad/glad.h>
#include "BloomPass.h"
#include "Renderer/Abstractions/GLLog.h"
#include "Core/Logger.h"

BloomPass::BloomPass(const QuadRenderer* quad) : m_Quad(quad), m_WindowWidth(0), m_WindowHeight(0), m_MipChainLength(0)
{
	m_DownsamplingShader = std::make_unique<Shader>("shaders/quad.vert", "shaders/bloom_downsample.frag");
	m_UpsamplingShader = std::make_unique<Shader>("shaders/quad.vert", "shaders/bloom_upsample.frag");
	m_Framebuffer = std::make_unique<Framebuffer>(nullptr);
}

BloomPass::~BloomPass()
{
	ClearTextures();
}

void BloomPass::Render(const RenderContext& context, const BloomConfig& config, const Texture* mainTexture)
{
	if (!config.enabled)
	{
		return;
	}

	if (context.viewport.width != m_WindowWidth || context.viewport.height != m_WindowHeight)
	{
		Resize(context.viewport.width, context.viewport.height);
	}

	if (config.mipChainLength != m_MipChainLength)
	{
		ChangeMipChainLength(config.mipChainLength);
	}

	mainTexture->Bind(0);

	RenderDownsamples();
	RenderUpsamples(config.filterRadius);
}

void BloomPass::Reload()
{
	m_DownsamplingShader->Reload();
	m_UpsamplingShader->Reload();
}

void BloomPass::Resize(int windowWidth, int windowHeight)
{
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	if (m_MipChainLength != 0)
	{
		ClearTextures();
		RecreateTexture();
	}
}

void BloomPass::ChangeMipChainLength(size_t newMipChainLength)
{
	m_MipChainLength = newMipChainLength;

	if (m_WindowWidth != 0 && m_WindowHeight != 0)
	{
		ClearTextures();
		RecreateTexture();
	}
}

void BloomPass::RenderDownsamples()
{
	m_DownsamplingShader->Bind();

	for (size_t i = 0; i < m_BloomTextures.size(); i++)
	{
		const Texture* tex = m_BloomTextures[i].get();

		m_DownsamplingShader->SetInt("u_ScreenTexture", 0);
		m_DownsamplingShader->SetVec2("u_Resolution", glm::vec2(tex->GetWidth(), tex->GetHeight()));

		RenderTexture(tex);

		tex->Bind(0); // Output of current pass becomes input for next pass
	}

	m_DownsamplingShader->Unbind();
}

void BloomPass::RenderUpsamples(float filterRadius)
{
	if (m_BloomTextures.size() < 2)
	{
		return;
	}

	m_UpsamplingShader->Bind();

	OPENGL_CALL(glEnable(GL_BLEND));
	OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE));
	OPENGL_CALL(glBlendEquation(GL_FUNC_ADD));

	for (size_t i = m_BloomTextures.size() - 1; i > 0; i--)
	{
		const Texture* tex = m_BloomTextures[i].get();
		const Texture* nextTex = m_BloomTextures[i - 1].get();

		tex->Bind(0); // Output of downsample pass becomes input for upsample pass

		m_UpsamplingShader->SetInt("u_ScreenTexture", 0);
		m_UpsamplingShader->SetFloat("u_FilterRadius", filterRadius);

		RenderTexture(nextTex);
	}

	OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	OPENGL_CALL(glDisable(GL_BLEND));

	m_UpsamplingShader->Unbind();
}

void BloomPass::RenderTexture(const Texture* texture)
{
	m_Framebuffer->Bind(texture->GetWidth(), texture->GetHeight());
	m_Framebuffer->SetTexture(texture);
	m_Quad->Draw();
	m_Framebuffer->Unbind();
}

void BloomPass::ClearTextures()
{
	m_BloomTextures.clear();

	spdlog::debug("Bloom textures were cleared");
}

void BloomPass::RecreateTexture()
{
	m_BloomTextures.reserve(m_MipChainLength);

	int texWidth = m_WindowWidth;
	int texHeight = m_WindowHeight;

	for (size_t i = 0; i < m_MipChainLength; i++)
	{
		texWidth /= 2;
		texHeight /= 2;

		m_BloomTextures.emplace_back(std::make_unique<Texture>(texWidth, texHeight));
	}

	spdlog::debug("Bloom textures were recreated with new size: {}x{} and mip chain length: {}", m_WindowWidth, m_WindowHeight, m_MipChainLength);
}