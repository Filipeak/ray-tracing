#include "Window.h"
#include "GLLog.h"
#include "../Core/Config.h"
#include <iostream>

Window::Window() : m_Window(0), m_Width(0), m_Height(0)
{
	if (!glfwInit())
	{
		std::cout << "Failed initializing GLFW" << std::endl;

		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLSL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLSL_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if (!m_Window)
	{
		std::cout << "Failed initializing window" << std::endl;

		glfwTerminate();

		return;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(VSYNC);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed initializing GLAD" << std::endl;

		return;
	}

	std::cout << "OpenGL Info:" << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
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

		OPENGL_CALL(glViewport(0, 0, m_Width, m_Height));

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
	glfwPollEvents();
}

GLFWwindow* Window::GetWindow() const
{
	return m_Window;
}

const int& Window::GetWidth() const
{
	return m_Width;
}

const int& Window::GetHeight() const
{
	return m_Height;
}