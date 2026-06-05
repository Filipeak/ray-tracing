#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/Config.h"

class Window
{
public:
	Window(int width, int height, const char* title, const WindowConfig& config);
	~Window();

	bool ShouldUpdate();
	void SwapBuffers();
	void PollEvents();

	GLFWwindow* GetWindowHandle() const;
	const int& GetWindowWidth() const;
	const int& GetWindowHeight() const;

private:
	GLFWwindow* m_Window;
	const WindowConfig& m_Config;
	int m_Width;
	int m_Height;
	bool m_VSync;

	void SetVSync(bool enabled);
};