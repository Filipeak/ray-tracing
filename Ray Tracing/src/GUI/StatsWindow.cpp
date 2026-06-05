#include "StatsWindow.h"
#include "imgui/imgui.h"

void StatsWindow::Draw()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Stats", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize({ 200, 75 });
	ImGui::SetWindowPos({ 10, 10 });
	ImGui::Text("%.3f FPS", 1.0f / m_Time.GetDeltaSeconds());
	ImGui::Text("Last Render: %.3f ms", m_Time.GetDeltaSeconds() * 1000.0f);
	ImGui::End();
}