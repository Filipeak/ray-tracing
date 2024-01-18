/**
* TODO:
*  [ ] Ray Tracing
*		- https://www.youtube.com/watch?v=A61S_2swwAc
*		- https://raytracing.github.io/books/RayTracingInOneWeekend.html
*		- https://raytracing.github.io/books/RayTracingTheNextWeek.html
*		- https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html
*  [ ] Illumination: Diffuse, Direct, Global, Specular
*  [ ] PBR
*		- https://www.youtube.com/watch?v=7_iy4Rov2Ck&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=10
*		- https://www.youtube.com/watch?v=AbVfW4X01a0&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=15
*		- https://learnopengl.com/PBR/Theory
*		- https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
*		- https://www.youtube.com/watch?v=XK_p2MxGBQs
*  [ ] Path Tracing
*		- https://www.youtube.com/watch?v=NIpC53vesHo&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=12
*  [ ] Camera / Scene abstraction
*  [ ] Antialiasing
*		- https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
*		- https://raytracing.github.io/books/RayTracingInOneWeekend.html#antialiasing
*  [ ] BVH Algorithm
*		- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume.html
*  [ ] CUDA / OptiX - Path Tracing, Mesh Ray Tracing
*  [ ] DirectX 11, 12 / Vulkan
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

	while (!glfwWindowShouldClose(window))
	{
		OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		OPENGL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}