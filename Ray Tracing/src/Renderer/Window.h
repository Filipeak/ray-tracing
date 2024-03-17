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

	GLFWwindow* GetWindowHandle() const;
	const int& GetViewportWidth() const;
	const int& GetViewportHeight() const;
	float GetDeltaTime() const;
	bool ViewportChanged() const;

private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	double m_LastTime;
	double m_DeltaTime;
	bool m_ViewportChanged;
	int m_LastWidth;
	int m_LastHeight;

	void PrintInfo() const;
};