#pragma once

#include <vector>
#include "GUIWindow.h"
#include "../Renderer/Window.h"

class GUI
{
public:
	GUI(const Window& window);
	~GUI();

	void AddWindow(GUIWindow* guiWindow);
	void Begin();
	void End();

private:
	void Update();

	std::vector<GUIWindow*> m_GUIWindows;
};