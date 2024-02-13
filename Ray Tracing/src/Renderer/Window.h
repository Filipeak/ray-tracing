#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	~Window();

	bool ShouldUpdate();
	void SwapBuffers();

	GLFWwindow* GetWindow() const;
	const int& GetWidth() const;
	const int& GetHeight() const;

private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
};