#pragma once

#include <memory>
#include <vector>
#include "IGUIWindow.h"
#include "Platform/Window.h"

class GUI
{
public:
	GUI(const Window& window);
	~GUI();

	void AddWindow(std::unique_ptr<IGUIWindow> guiWindow);
	void Update();

private:
	std::vector<std::unique_ptr<IGUIWindow>> m_GUIWindows;
};