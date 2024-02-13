#pragma once

#include "../Renderer/Window.h"

class GUI
{
public:
	GUI(const Window& window);
	~GUI();

	void Begin();
	void End();

private:
	void Update();
};