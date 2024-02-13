#include "Camera.h"
#include "../Core/Config.h"
#include <GLFW/glfw3.h>

Camera::Camera() : m_Position(0, 0, -3), m_Rotation(0)
{
}

void Camera::Update(const Window& window)
{
	glm::vec3 input(0);
	float mulitplier = 1;

	if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		input.z += 1;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		input.z -= 1;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		input.x -= 1;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		input.x += 1;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		input.y -= 1;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
	{
		input.y += 1;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mulitplier = 2;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		input.y += 1;
	}

	if (glfwGetMouseButton(window.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window.GetWindow(), &mouseX, &mouseY);

		float normX = (mouseX - window.GetWidth() / 2.0f) / window.GetWidth();
		float normY = (mouseY - window.GetHeight() / 2.0f) / window.GetHeight();
	}

	static float lastTime = 0;
	float deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();

	if (input.x != 0 || input.y != 0 || input.z != 0)
	{
		m_Position += glm::normalize(input) * mulitplier * deltaTime * CAMERA_SPEED;
	}
}

const glm::vec3& Camera::GetPos() const
{
	return m_Position;
}