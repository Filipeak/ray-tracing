#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, const char* title, bool vsync);
	~Window();

	bool ShouldUpdate();
	void SwapBuffers();

	GLFWwindow* GetWindowHandle() const;
	const int& GetWindowWidth() const;
	const int& GetWindowHeight() const;
	float GetDeltaTime() const;
	bool SizeChanged() const;

	bool IsVSyncEnabled() const;
	void SetVSync(bool vsync);

private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	int m_LastWidth;
	int m_LastHeight;
	double m_LastTime;
	double m_DeltaTime;
	bool m_SizeChanged;
	bool m_VSync;
};