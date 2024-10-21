#include "GUI.h"
#include "StatsWindow.h"
#include "ConfigWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GUI::GUI(const Window& window)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.GetWindowHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 430");
}

GUI::~GUI()
{
	for (size_t i = 0; i < m_GUIWindows.size(); i++)
	{
		delete m_GUIWindows[i];
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::AddWindow(GUIWindow* guiWindow)
{
	m_GUIWindows.emplace_back(guiWindow);
}

void GUI::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	Update();
}

void GUI::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Update()
{
	for (size_t i = 0; i < m_GUIWindows.size(); i++)
	{
		m_GUIWindows[i]->Draw();
	}
}