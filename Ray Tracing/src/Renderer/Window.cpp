#include "Window.h"
#include "GLLog.h"
#include <iostream>

Window::Window(int width, int height, const char* title, bool vsync) : m_Window(nullptr), m_Width(width), m_Height(height), m_LastWidth(width), m_LastHeight(height), m_LastTime(0), m_DeltaTime(0), m_SizeChanged(false), m_VSync(vsync)
{
	if (!glfwInit())
	{
		std::cout << "Failed initializing GLFW" << std::endl;

		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, title, NULL, NULL);

	if (!m_Window)
	{
		std::cout << "Failed initializing window" << std::endl;

		glfwTerminate();

		return;
	}

	glfwMakeContextCurrent(m_Window);

	SetVSync(m_VSync);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed initializing GLAD" << std::endl;

		return;
	}

	std::cout << "OpenGL Info:" << std::endl;
	OPENGL_CALL(std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl);
	OPENGL_CALL(std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl);
	OPENGL_CALL(std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool Window::ShouldUpdate()
{
	if (!glfwWindowShouldClose(m_Window))
	{
		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

		if (m_Width == 0 || m_Height == 0)
		{
			m_Width = m_LastWidth;
			m_Height = m_LastHeight;
		}

		m_SizeChanged = false;

		if (m_LastWidth != m_Width || m_LastHeight != m_Height)
		{
			std::cout << "Window size has been changed to: " << m_Width << " " << m_Height << std::endl;

			m_SizeChanged = true;
			m_LastWidth = m_Width;
			m_LastHeight = m_Height;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void Window::SwapBuffers()
{
	m_DeltaTime = glfwGetTime() - m_LastTime;
	m_LastTime = glfwGetTime();

	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

GLFWwindow* Window::GetWindowHandle() const
{
	return m_Window;
}

const int& Window::GetWindowWidth() const
{
	return m_Width;
}

const int& Window::GetWindowHeight() const
{
	return m_Height;
}

float Window::GetDeltaTime() const
{
	return (float)m_DeltaTime;
}

bool Window::SizeChanged() const
{
	return m_SizeChanged;
}

bool Window::IsVSyncEnabled() const
{
	return m_VSync;
}

void Window::SetVSync(bool vsync)
{
	m_VSync = vsync;

	glfwSwapInterval((int)m_VSync);
}