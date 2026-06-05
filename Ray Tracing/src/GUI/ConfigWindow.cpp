#include "ConfigWindow.h"
#include "imgui/imgui.h"

void ConfigWindow::Draw()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Config", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize({ 450, 290 });
	ImGui::SetWindowPos({ 10, 780 });

	ImGui::SliderFloat("HDR Exposure", &m_ToneMapConfig.exposure, 0.0f, 10.0f, "%.1f");
	ImGui::SliderFloat("Screen Gamma", &m_ToneMapConfig.gamma, 1.0f, 2.5f, "%.1f");
	ImGui::Checkbox("VSync", &m_WindowConfig.vsync);
	ImGui::Checkbox("Accumulate", &m_RayTracingConfig.accumulate);
	ImGui::Checkbox("Bloom", &m_BloomConfig.enabled);
	ImGui::SliderInt("Bloom Mip Chains", &m_BloomConfig.mipChainLength, 1, 10);
	ImGui::SliderFloat("Bloom Strength", &m_BloomConfig.strength, 0.0f, 0.5f, "%.3f");
	ImGui::SliderFloat("Bloom Filter Radius", &m_BloomConfig.filterRadius, 0.0f, 0.5f, "%.3f");
	ImGui::SliderFloat("Camera FOV", &m_ProjectionSettings.fov, 20.0f, 120.0f, "%.0f");
	ImGui::SliderFloat("Camera Speed", &m_CameraSettings.speed, 1.0f, 20.0f, "%.1f");
	ImGui::SliderFloat("Camera Sensitivity", &m_CameraSettings.sensitivity, 0.5f, 10.0f, "%.1f");

	ImGui::End();
}