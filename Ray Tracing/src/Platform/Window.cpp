#include "Window.h"
#include "Core/Logger.h"
#include "Core/EngineAssert.h"

Window::Window(int width, int height, const char* title, const WindowConfig& config) : m_Window(nullptr), m_Width(width), m_Height(height), m_Config(config), m_VSync(config.vsync)
{
	if (!glfwInit())
	{
		ENGINE_ASSERT(false, "Failed to initialize GLFW");

		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, title, NULL, NULL);

	if (!m_Window)
	{
		ENGINE_ASSERT(false, "Failed initializing window");

		glfwTerminate();

		return;
	}

	glfwMakeContextCurrent(m_Window);

	SetVSync(m_VSync);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ENGINE_ASSERT(false, "Failed initializing GLAD");

		return;
	}

	spdlog::info("Successfully initialized window and OpenGL context");
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

		if (m_VSync != m_Config.vsync)
		{
			SetVSync(m_Config.vsync);
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
	glfwSwapBuffers(m_Window);
}

void Window::PollEvents()
{
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

void Window::SetVSync(bool vsync)
{
	m_VSync = vsync;

	glfwSwapInterval((int)m_VSync);

	spdlog::debug("VSync {}", m_VSync ? "enabled" : "disabled");
}