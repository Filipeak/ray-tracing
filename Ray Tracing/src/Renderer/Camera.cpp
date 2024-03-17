#include "Camera.h"
#include <GLFW/glfw3.h>

static constexpr float MAX_PITCH_RAD = glm::pi<float>() / 3.0f;
static constexpr glm::vec3 UP_DIR = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(const Window& window, float verticalFOV, float near, float far, float speed, float sensitivity)
	: m_Window(window), m_VerticalFOV(verticalFOV), m_NearClip(near), m_FarClip(far), m_Speed(speed), m_Sensitivity(sensitivity)
{
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(window.GetWindowHandle(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	RecalculateView();
}

void Camera::Update()
{
	glm::vec3 input(0.0f);
	float mulitplier = 1.0f;
	bool moved = false;

	glm::vec3 cameraRight = glm::normalize(glm::cross(m_ForwardDirection, UP_DIR));
	m_UpDirection = glm::normalize(glm::cross(cameraRight, m_ForwardDirection));

	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_W) == GLFW_PRESS)
	{
		input += m_ForwardDirection;
	}
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_S) == GLFW_PRESS)
	{
		input -= m_ForwardDirection;
	}
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_A) == GLFW_PRESS)
	{
		input -= cameraRight;
	}
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
	{
		input += cameraRight;
	}
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		input -= m_UpDirection;
	}
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_E) == GLFW_PRESS)
	{
		input += m_UpDirection;
	}
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mulitplier = 2.0f;
	}

	if (glfwGetMouseButton(m_Window.GetWindowHandle(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(m_Window.GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		double mouseX, mouseY;
		glfwGetCursorPos(m_Window.GetWindowHandle(), &mouseX, &mouseY);

		float yaw = ((float)mouseX - m_Window.GetViewportWidth() / 2.0f) / m_Window.GetViewportWidth();
		float pitch = (m_Window.GetViewportHeight() / 2.0f - (float)mouseY) / m_Window.GetViewportHeight();

		if (m_FirstCameraClick)
		{
			float deltaPitch = m_Sensitivity * (pitch - m_LastMousePosition.x);
			float deltaYaw = m_Sensitivity * (yaw - m_LastMousePosition.y);

			if (deltaPitch != 0 || deltaYaw != 0)
			{
				m_Rotation.x += deltaPitch;
				m_Rotation.y += deltaYaw;

				m_Rotation.x = glm::clamp(m_Rotation.x, -MAX_PITCH_RAD, MAX_PITCH_RAD);

				float sin_yaw = sinf(-m_Rotation.y);
				float cos_yaw = cosf(-m_Rotation.y);
				float sin_pitch = sinf(m_Rotation.x);
				float cos_pitch = cosf(m_Rotation.x);

				m_ForwardDirection = glm::vec3(sin_yaw * cos_pitch, sin_pitch, cos_yaw * cos_pitch);

				moved = true;
			}
		}

		m_FirstCameraClick = true;
		m_LastMousePosition.x = pitch;
		m_LastMousePosition.y = yaw;
	}
	else if (glfwGetMouseButton(m_Window.GetWindowHandle(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(m_Window.GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		m_FirstCameraClick = false;
	}

	if (input.x != 0 || input.y != 0 || input.z != 0)
	{
		m_Position += glm::normalize(input) * m_Window.GetDeltaTime() * mulitplier * m_Speed;

		moved = true;
	}

	if (moved)
	{
		RecalculateView();
	}

	TryRecalculateProjection();
}

const glm::vec3& Camera::GetPosition() const
{
	return m_Position;
}

const glm::mat4x4& Camera::GetInverseProjectionMatrix() const
{
	return m_InverseProjection;
}

const glm::mat4x4& Camera::GetInverseViewMatrix() const
{
	return m_InverseView;
}

void Camera::TryRecalculateProjection()
{
	if (m_Window.ViewportChanged())
	{
		glm::mat4x4 perspective = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_Window.GetViewportWidth(), (float)m_Window.GetViewportHeight(), m_NearClip, m_FarClip);
		
		m_InverseProjection = glm::inverse(perspective);
	}
}

void Camera::RecalculateView()
{
	glm::mat4x4 view = glm::lookAt(m_Position, m_Position + m_ForwardDirection, m_UpDirection);

	m_InverseView = glm::inverse(view);
}