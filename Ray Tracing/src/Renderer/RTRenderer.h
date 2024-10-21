#pragma once

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
	RTRenderer(const Window& window, Camera& camera, bool accumulate, float hdrExposure, float gamma, bool useBloom, float bloomStength, float bloomFilterRadius);
	~RTRenderer();

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
	QuadRenderer* m_Quad;	
	Bloom* m_Bloom;
	Framebuffer* m_RTFramebuffer;
	Texture* m_ScreenTexture1;
	Texture* m_ScreenTexture2;
	Shader* m_MainShader;
	Shader* m_ScreenShader;
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
	void RenderMainFrame();
	void RenderPostProcessing();
};