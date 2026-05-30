#pragma once

#include <memory>
#include "Window.h"
#include "QuadRenderer.h"
#include "Bloom.h"
#include "Camera.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Texture.h"

class RTRenderer
{
public:
	RTRenderer(const Window& window, Camera& camera, bool accumulate, float hdrExposure, float gamma, bool useBloom, int bloomMipChains, float bloomStrength, float bloomFilterRadius);

	void Update();
	void SetAccumulate(bool accumulate);
	void SetHDRExposure(float exposure);
	void SetScreenGamma(float gamma);
	void SetBloom(bool bloom);
	void SetBloomMipChains(int mipChains);
	void SetBloomStrength(float strength);
	void SetBloomFilterRadius(float radius);

private:
	const Window& m_Window;
	Camera& m_Camera;

	std::shared_ptr<QuadRenderer> m_Quad;	
	std::unique_ptr<Bloom> m_Bloom;
	std::unique_ptr<Framebuffer> m_RTFramebuffer;
	std::unique_ptr<Texture> m_ScreenTexture1;
	std::unique_ptr<Texture> m_ScreenTexture2;
	std::unique_ptr<Shader> m_MainShader;
	std::unique_ptr<Shader> m_ScreenShader;
	int m_CurrentBackupTextureSlot;
	int m_CurrentScreenTextureSlot;

	bool m_ReloadKeyPressTest;
	bool m_PingPongBufferSelect;
	int m_AccumulationPasses;
	unsigned int m_FrameIndex;
	bool m_Accumulate;
	float m_HdrExposure;
	float m_Gamma;
	bool m_UseBloom;
	float m_BloomStrength;
	float m_BloomFilterRadius;

	void HandleReload();
	void HandleRescale();
	void HandleCamera();
	void UpdatePingPongBufferSelect();
	void RenderMainFrame();
	void RenderPostProcessing();
	void RenderFinal();
};