#include "RTRenderer.h"
#include "GLLog.h"
#include <iostream>

#define SCREEN_TEXTURE_1_SLOT 0
#define SCREEN_TEXTURE_2_SLOT 1
#define BLOOM_TEXTURE_SLOT 2

RTRenderer::RTRenderer(const Window& window, Camera& camera, bool accumulate, float hdrExposure, float gamma, bool useBloom, int bloomMipChains, float bloomStrength, float bloomFilterRadius) : m_Window(window), m_Camera(camera), m_CurrentBackupTextureSlot(0), m_CurrentScreenTextureSlot(0), m_ReloadKeyPressTest(false), m_PingPongBufferSelect(true), m_AccumulationPasses(0), m_FrameIndex(1), m_Accumulate(accumulate), m_HdrExposure(hdrExposure), m_Gamma(gamma), m_UseBloom(useBloom), m_BloomStrength(bloomStrength), m_BloomFilterRadius(bloomFilterRadius)
{
	m_Quad = std::make_shared<QuadRenderer>();
	m_Bloom = std::make_unique<Bloom>(m_Window.GetWindowWidth(), m_Window.GetWindowHeight(), BLOOM_TEXTURE_SLOT, bloomMipChains, m_Quad);
	m_MainShader = std::make_unique<Shader>("shaders/quad.vert", "shaders/raytracing.frag");
	m_ScreenShader = std::make_unique<Shader>("shaders/quad.vert", "shaders/screen.frag");
	m_ScreenTexture1 = std::make_unique<Texture>(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());
	m_ScreenTexture2 = std::make_unique<Texture>(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());
	m_RTFramebuffer = std::make_unique<Framebuffer>(m_ScreenTexture1.get());

	Texture::SelectSlot(SCREEN_TEXTURE_1_SLOT);
	m_ScreenTexture1->Bind();

	Texture::SelectSlot(SCREEN_TEXTURE_2_SLOT);
	m_ScreenTexture2->Bind();
}

void RTRenderer::Update()
{
	HandleReload();
	HandleRescale();
	HandleCamera();

	UpdatePingPongBufferSelect();

	RenderMainFrame();
	RenderPostProcessing();
	RenderFinal();
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
		Texture::SelectSlot(SCREEN_TEXTURE_1_SLOT);
		m_ScreenTexture1->Bind();
		m_ScreenTexture1->SetupCurrentTexture(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

		Texture::SelectSlot(SCREEN_TEXTURE_2_SLOT);
		m_ScreenTexture2->Bind();
		m_ScreenTexture2->SetupCurrentTexture(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

		m_Bloom->Rescale(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

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

void RTRenderer::UpdatePingPongBufferSelect()
{
	m_CurrentScreenTextureSlot = m_PingPongBufferSelect ? SCREEN_TEXTURE_1_SLOT : SCREEN_TEXTURE_2_SLOT;
	m_CurrentBackupTextureSlot = m_PingPongBufferSelect ? SCREEN_TEXTURE_2_SLOT : SCREEN_TEXTURE_1_SLOT;

	m_PingPongBufferSelect = !m_PingPongBufferSelect;
}

void RTRenderer::RenderMainFrame()
{
	Framebuffer::SetGlobalViewportSize(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

	m_RTFramebuffer->Bind();
	m_RTFramebuffer->SetTexture(m_CurrentScreenTextureSlot == SCREEN_TEXTURE_1_SLOT ? m_ScreenTexture1.get() : m_ScreenTexture2.get());
	m_MainShader->Bind();
	m_MainShader->SetInt("u_ScreenTexture", m_CurrentBackupTextureSlot);
	m_MainShader->SetUInt("u_AccumulationPasses", m_AccumulationPasses);
	m_MainShader->SetVec2("u_Resolution", glm::vec2(m_Window.GetWindowWidth(), m_Window.GetWindowHeight()));
	m_MainShader->SetUInt("u_FrameIndex", m_FrameIndex);
	m_MainShader->SetVec3("u_CameraPosition", m_Camera.GetPosition());
	m_MainShader->SetMat4x4("u_CameraInverseProjection", m_Camera.GetInverseProjectionMatrix());
	m_MainShader->SetMat4x4("u_CameraInverseView", m_Camera.GetInverseViewMatrix());
	m_Quad->Draw();
	m_RTFramebuffer->Unbind();

	m_AccumulationPasses = m_Accumulate ? m_AccumulationPasses + 1 : 0;
	m_FrameIndex++;
}

void RTRenderer::RenderPostProcessing()
{
	if (m_UseBloom)
	{
		m_Bloom->Render(m_CurrentScreenTextureSlot, m_BloomFilterRadius);
	}
}

void RTRenderer::RenderFinal()
{
	Framebuffer::SetGlobalViewportSize(m_Window.GetWindowWidth(), m_Window.GetWindowHeight());

	m_ScreenShader->Bind();
	m_ScreenShader->SetInt("u_ScreenTexture", m_CurrentScreenTextureSlot);
	m_ScreenShader->SetInt("u_BloomTexture", BLOOM_TEXTURE_SLOT);
	m_ScreenShader->SetInt("u_BloomEnabled", (int)m_UseBloom);
	m_ScreenShader->SetFloat("u_BloomStrength", m_BloomStrength);
	m_ScreenShader->SetFloat("u_HdrExposure", m_HdrExposure);
	m_ScreenShader->SetFloat("u_Gamma", m_Gamma);
	m_Quad->Draw();
}