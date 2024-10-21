#include "RTRenderer.h"
#include "GLLog.h"
#include <iostream>

#define SCREEN_TEXTURE_1_SLOT 0
#define SCREEN_TEXTURE_2_SLOT 1
#define BLOOM_TEXTURE_SLOT 2

RTRenderer::RTRenderer(const Window& window, Camera& camera, bool accumulate, float hdrExposure, float gamma, bool useBloom, float bloomStength, float bloomFilterRadius) : m_Window(window), m_Camera(camera), m_ReloadKeyPressTest(false), m_PingPongBufferSelect(false), m_AccumulationPasses(0), m_FrameIndex(1), m_Accumulate(accumulate), m_HdrExposure(hdrExposure), m_Gamma(gamma), m_UseBloom(useBloom), m_BloomStrength(bloomStength), m_BloomFilterRadius(bloomFilterRadius)
{
	m_Quad = new QuadRenderer();
	m_Bloom = new Bloom(m_Window.GetWindowWidth(), m_Window.GetWindowHeight(), 5, m_Quad);

	m_MainShader = new Shader("shaders/quad.vert", "shaders/raytracing.frag");
	m_ScreenShader = new Shader("shaders/quad.vert", "shaders/screen.frag");

	m_ScreenTexture1 = new Texture(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());
	m_ScreenTexture2 = new Texture(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());
	m_RTFramebuffer = new Framebuffer(m_ScreenTexture1);

	Texture::SelectSlot(SCREEN_TEXTURE_1_SLOT);
	m_ScreenTexture1->Bind();
	Texture::SelectSlot(SCREEN_TEXTURE_2_SLOT);
	m_ScreenTexture2->Bind();
}

RTRenderer::~RTRenderer()
{
	delete m_Quad;
	delete m_Bloom;
	delete m_MainShader;
	delete m_ScreenShader;
	delete m_RTFramebuffer;
	delete m_ScreenTexture1;
	delete m_ScreenTexture2;
}

void RTRenderer::Update()
{
	HandleReload();
	HandleRescale();
	HandleCamera();

	RenderMainFrame();
	RenderPostProcessing();
}

void RTRenderer::SetAccumulate(bool accumulate)
{
	m_Accumulate = accumulate;

	if (!m_Accumulate)
	{
		m_AccumulationPasses = 0;
	}
}

void RTRenderer::SetHDRExposure(float exposure)
{
	m_HdrExposure = exposure;
}

void RTRenderer::SetScreenGamma(float gamma)
{
	m_Gamma = gamma;
}

void RTRenderer::SetBloom(bool bloom)
{
	m_UseBloom = bloom;
}

void RTRenderer::SetBloomMipChains(int mipChains)
{
	m_Bloom->ChangeMipChainLength(mipChains);
}

void RTRenderer::SetBloomStrength(float strength)
{
	m_BloomStrength = strength;
}

void RTRenderer::SetBloomFilterRadius(float radius)
{
	m_BloomFilterRadius = radius;
}

void RTRenderer::HandleReload()
{
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_R) == GLFW_PRESS)
	{
		if (!m_ReloadKeyPressTest)
		{
			std::cout << "Reload requested" << std::endl;

			m_MainShader->Reload();
			m_ScreenShader->Reload();
			m_Bloom->Reload();

			m_ReloadKeyPressTest = true;
			m_AccumulationPasses = 0;
		}
	}
	else
	{
		m_ReloadKeyPressTest = false;
	}
}

void RTRenderer::HandleRescale()
{
	if (m_Window.SizeChanged())
	{
		m_ScreenTexture1->Rescale(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());
		m_ScreenTexture2->Rescale(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());
		m_Bloom->Rescale(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

		Texture::SelectSlot(SCREEN_TEXTURE_1_SLOT);
		m_ScreenTexture1->Bind();
		Texture::SelectSlot(SCREEN_TEXTURE_2_SLOT);
		m_ScreenTexture2->Bind();

		m_AccumulationPasses = 0;

		std::cout << "Framebuffers and textures were rescaled" << std::endl;
	}
}

void RTRenderer::HandleCamera()
{
	m_Camera.Update();

	if (m_Camera.HasMoved())
	{
		m_AccumulationPasses = 0;
	}
}

void RTRenderer::RenderMainFrame()
{
	Framebuffer::SetViewportSize(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

	m_RTFramebuffer->Bind();
	m_RTFramebuffer->SetTexture(!m_PingPongBufferSelect ? m_ScreenTexture1 : m_ScreenTexture2);

	m_MainShader->Bind();
	m_MainShader->SetInt("u_ScreenTexture", !m_PingPongBufferSelect ? SCREEN_TEXTURE_2_SLOT : SCREEN_TEXTURE_1_SLOT);
	m_MainShader->SetUInt("u_AccumulationPasses", m_AccumulationPasses);
	m_MainShader->SetVec2("u_Resolution", glm::vec2(m_Window.GetWindowWidth(), m_Window.GetWindowHeight()));
	m_MainShader->SetUInt("u_FrameIndex", m_FrameIndex);
	m_MainShader->SetVec3("u_CameraPosition", m_Camera.GetPosition());
	m_MainShader->SetMat4x4("u_CameraInverseProjection", m_Camera.GetInverseProjectionMatrix());
	m_MainShader->SetMat4x4("u_CameraInverseView", m_Camera.GetInverseViewMatrix());
	m_Quad->Draw();

	m_RTFramebuffer->Unbind();


	m_PingPongBufferSelect = !m_PingPongBufferSelect;

	if (m_Accumulate)
	{
		m_AccumulationPasses++;
	}
	else
	{
		m_AccumulationPasses = 0;
	}

	m_FrameIndex++;
}

void RTRenderer::RenderPostProcessing()
{
	int screenTexture = m_PingPongBufferSelect ? SCREEN_TEXTURE_1_SLOT : SCREEN_TEXTURE_2_SLOT;

	if (m_UseBloom)
	{
		m_Bloom->Render(screenTexture, BLOOM_TEXTURE_SLOT, m_BloomFilterRadius);
	}

	Framebuffer::SetViewportSize(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

	m_ScreenShader->Bind();
	m_ScreenShader->SetInt("u_ScreenTexture", screenTexture);
	m_ScreenShader->SetInt("u_BloomTexture", BLOOM_TEXTURE_SLOT);
	m_ScreenShader->SetInt("u_BloomEnabled", (int)m_UseBloom);
	m_ScreenShader->SetFloat("u_BloomStrength", m_BloomStrength);
	m_ScreenShader->SetFloat("u_HdrExposure", m_HdrExposure);
	m_ScreenShader->SetFloat("u_Gamma", m_Gamma);
	m_Quad->Draw();
}