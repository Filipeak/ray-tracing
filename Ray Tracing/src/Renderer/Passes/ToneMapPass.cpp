#include "ToneMapPass.h"
#include "Renderer/Abstractions/Framebuffer.h"

ToneMapPass::ToneMapPass(const QuadRenderer* quad) : m_Quad(quad)
{
	m_ScreenShader = std::make_unique<Shader>("shaders/quad.vert", "shaders/screen.frag");
}

void ToneMapPass::Render(const RenderContext& context, const BloomConfig& bloomConfig, const ToneMapConfig& config, const Texture* mainTexture, const Texture* bloomTexture)
{
	// Framebuffer setup
	Framebuffer::BindDefault(context.viewport.width, context.viewport.height);

	// Bind
	mainTexture->Bind(0);
	
	if (bloomTexture) 
	{
		bloomTexture->Bind(1);
	}

	m_ScreenShader->Bind();
	
	// Draw
	m_ScreenShader->SetInt("u_ScreenTexture", 0);
	m_ScreenShader->SetInt("u_BloomTexture", 1);
	m_ScreenShader->SetInt("u_BloomEnabled", (int)(bloomConfig.enabled));
	m_ScreenShader->SetFloat("u_BloomStrength", bloomConfig.strength);
	m_ScreenShader->SetFloat("u_HdrExposure", config.exposure);
	m_ScreenShader->SetFloat("u_Gamma", config.gamma);
	m_Quad->Draw();

	// Unbind
	m_ScreenShader->Unbind();

	mainTexture->Unbind();

	if (bloomTexture) 
	{
		bloomTexture->Unbind();
	}
}

void ToneMapPass::Reload()
{
	m_ScreenShader->Reload();
}