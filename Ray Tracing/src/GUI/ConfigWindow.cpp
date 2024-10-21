#include "ConfigWindow.h"
#include "imgui/imgui.h"

void ConfigWindow::Draw()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize({ 450, 290 });
	ImGui::SetWindowPos({ 10, 780 });

	if (ImGui::SliderFloat("HDR Exposure", &m_ConfigData.hdrExposure, 0.0f, 10.0f, "%.1f"))
	{
		m_Renderer.SetHDRExposure(m_ConfigData.hdrExposure);
	}

	if (ImGui::SliderFloat("Screen Gamma", &m_ConfigData.screenGamma, 1.0f, 2.5f, "%.1f"))
	{
		m_Renderer.SetScreenGamma(m_ConfigData.screenGamma);
	}

	if (ImGui::Checkbox("VSync", &m_ConfigData.vsync))
	{
		m_Window.SetVSync(m_ConfigData.vsync);
	}

	if (ImGui::Checkbox("Accumulate", &m_ConfigData.accumulate))
	{
		m_Renderer.SetAccumulate(m_ConfigData.accumulate);
	}

	if (ImGui::Checkbox("Bloom", &m_ConfigData.bloom))
	{
		m_Renderer.SetBloom(m_ConfigData.bloom);
	}

	if (ImGui::SliderInt("Bloom Mip Chains", &m_ConfigData.bloomMipChains, 1, 10))
	{
		m_Renderer.SetBloomMipChains(m_ConfigData.bloomMipChains);
	}

	if (ImGui::SliderFloat("Bloom Strength", &m_ConfigData.bloomStrength, 0.0f, 0.5f, "%.3f"))
	{
		m_Renderer.SetBloomStrength(m_ConfigData.bloomStrength);
	}

	if (ImGui::SliderFloat("Bloom Filter Radius", &m_ConfigData.bloomFilterRadius, 0.0f, 0.5f, "%.3f"))
	{
		m_Renderer.SetBloomFilterRadius(m_ConfigData.bloomFilterRadius);
	}

	if (ImGui::SliderFloat("Camera FOV", &m_ConfigData.cameraFOV, 20.0f, 120.0f, "%.0f"))
	{
		m_Camera.SetFOV(m_ConfigData.cameraFOV);
	}

	if (ImGui::SliderFloat("Camera Speed", &m_ConfigData.cameraSpeed, 1.0f, 20.0f, "%.1f"))
	{
		m_Camera.SetSpeed(m_ConfigData.cameraSpeed);
	}

	if (ImGui::SliderFloat("Camera Sensitivity", &m_ConfigData.cameraSens, 0.5f, 10.0f, "%.1f"))
	{
		m_Camera.SetSens(m_ConfigData.cameraSens);
	}

	ImGui::End();
}