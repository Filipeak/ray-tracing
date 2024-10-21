#include "Bloom.h"
#include "GLLog.h"

Bloom::Bloom(int windowWidth, int windowHeight, size_t mipChainLength, const QuadRenderer* quad) : m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), m_MipChainLength(mipChainLength), m_Quad(quad)
{
	m_DownsamplingShader = new Shader("shaders/quad.vert", "shaders/bloom_downsample.frag");
	m_UpsamplingShader = new Shader("shaders/quad.vert", "shaders/bloom_upsample.frag");

	RecreateTexture();
}

Bloom::~Bloom()
{
	delete m_DownsamplingShader;
	delete m_UpsamplingShader;

	ClearTextures();
}

void Bloom::Render(int srcTextureId, int bloomTextureId, float filterRadius)
{
	Texture::SelectSlot(bloomTextureId);

	m_Framebuffer->Bind();

	RenderDownsamples(srcTextureId, bloomTextureId);
	RenderUpsamples(bloomTextureId, filterRadius);

	m_Framebuffer->Unbind();

	m_BloomTextures[0]->Bind();
}

void Bloom::Reload()
{
	m_DownsamplingShader->Reload();
	m_UpsamplingShader->Reload();
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

void Bloom::RenderDownsamples(int srcTextureId, int bloomTextureId)
{
	m_DownsamplingShader->Bind();

	int currentTexture = srcTextureId;

	for (size_t i = 0; i < m_BloomTextures.size(); i++)
	{
		const Texture* tex = m_BloomTextures[i];

		m_DownsamplingShader->SetInt("u_ScreenTexture", currentTexture);
		m_DownsamplingShader->SetVec2("u_Resolution", glm::vec2(tex->GetWidth(), tex->GetHeight()));

		Framebuffer::SetViewportSize(tex->GetWidth(), tex->GetHeight());
		m_Framebuffer->SetTexture(tex);

		m_Quad->Draw();

		tex->Bind();
		currentTexture = bloomTextureId;
	}
}

void Bloom::RenderUpsamples(int bloomTextureId, float filterRadius)
{
	m_UpsamplingShader->Bind();

	OPENGL_CALL(glEnable(GL_BLEND));
	OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE));
	OPENGL_CALL(glBlendEquation(GL_FUNC_ADD));

	for (size_t i = m_BloomTextures.size() - 1; i > 0; i--)
	{
		const Texture* tex = m_BloomTextures[i];
		const Texture* nextTex = m_BloomTextures[i - 1];

		tex->Bind();

		m_UpsamplingShader->SetInt("u_ScreenTexture", bloomTextureId);
		m_UpsamplingShader->SetFloat("u_FilterRadius", filterRadius);

		Framebuffer::SetViewportSize(nextTex->GetWidth(), nextTex->GetHeight());
		m_Framebuffer->SetTexture(nextTex);

		m_Quad->Draw();
	}

	OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	OPENGL_CALL(glDisable(GL_BLEND));
}

void Bloom::ClearTextures()
{
	delete m_Framebuffer;

	for (size_t i = 0; i < m_BloomTextures.size(); i++)
	{
		delete m_BloomTextures[i];
	}

	m_BloomTextures.clear();
}

void Bloom::RecreateTexture()
{
	m_BloomTextures.reserve(m_MipChainLength);

	int texWidth = m_WindowWidth;
	int texHeight = m_WindowHeight;

	for (size_t i = 0; i < m_MipChainLength; i++)
	{
		texWidth /= 2;
		texHeight /= 2;

		Texture* t = new Texture(texWidth, texHeight);

		m_BloomTextures.emplace_back(t);
	}

	m_Framebuffer = new Framebuffer(m_BloomTextures[0]);
}