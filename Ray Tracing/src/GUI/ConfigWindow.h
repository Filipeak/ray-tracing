#pragma once

#include "GUIWindow.h"
#include "../Core/Config.h"
#include "../Renderer/Window.h"
#include "../Renderer/RTRenderer.h"
#include "../Renderer/Camera.h"

class ConfigWindow : public GUIWindow
{
public:
	ConfigWindow(ConfigData& configData, Window& window, RTRenderer& renderer, Camera& camera) : m_ConfigData(configData), m_Window(window), m_Renderer(renderer), m_Camera(camera) {}

	void Draw();

private:
	ConfigData& m_ConfigData;
	Window& m_Window;
	RTRenderer& m_Renderer;
	Camera& m_Camera;
};