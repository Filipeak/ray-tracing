#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/GLLog.h"

#define WIDTH 1920
#define HEIGHT 1080

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed initializing GLFW" << std::endl;

		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracing", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed initializing window" << std::endl;

		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed initializing GLAD" << std::endl;

		return -1;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices) / sizeof(float));
	IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

	va.PushLayoutFloat(3);
	va.SaveLayout();

	Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
	shader.Bind();
	shader.SetUniform2f("u_Resolution", WIDTH, HEIGHT);

	auto startTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fsec = currentTime - startTime;

		//shader.SetUniform1f("u_Time", fsec.count());

		OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		OPENGL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}