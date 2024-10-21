#define GLM_FORCE_LEFT_HANDED

#include "Camera.h"
#include <GLFW/glfw3.h>

static constexpr float MAX_PITCH_RAD = glm::pi<float>() / 3.0f;
static constexpr glm::vec3 UP_DIR = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(const Window& window, glm::vec3 startPosition, glm::vec2 startRotation, float verticalFOV, float near, float far, float speed, float sensitivity) : m_Window(window), m_VerticalFOV(verticalFOV), m_NearClip(near), m_FarClip(far), m_Speed(speed), m_Sensitivity(sensitivity)
{
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(window.GetWindowHandle(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	m_Position = startPosition;
	m_Rotation = startRotation;

	RecalculateVectors();
	RecalculateView();
	RecalculateProjection();
}

/*
 * REF: https://learnopengl.com/Getting-started/Camera
 * REF: https://learnopengl.com/Getting-started/Coordinate-Systems
 * REF: https://www.youtube.com/watch?v=lXlXqUEEJ94&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=7
 */
void Camera::Update()
{
	m_Moved = false;

	if (glfwGetMouseButton(m_Window.GetWindowHandle(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(m_Window.GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		double mouseX, mouseY;
		glfwGetCursorPos(m_Window.GetWindowHandle(), &mouseX, &mouseY);

		float yaw = ((float)mouseX - m_Window.GetWindowWidth() / 2.0f) / m_Window.GetWindowWidth();
		float pitch = (m_Window.GetWindowHeight() / 2.0f - (float)mouseY) / m_Window.GetWindowHeight();

		if (m_FirstCameraClick)
		{
			float deltaPitch = m_Sensitivity * (pitch - m_LastMousePosition.x);
			float deltaYaw = m_Sensitivity * (yaw - m_LastMousePosition.y);

			if (deltaPitch != 0 || deltaYaw != 0)
			{
				m_Rotation.x += deltaPitch;
				m_Rotation.y += deltaYaw;

				m_Rotation.x = glm::clamp(m_Rotation.x, -MAX_PITCH_RAD, MAX_PITCH_RAD);

				RecalculateVectors();

				m_Moved = true;
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

	float mulitplier = 1.0f;
	glm::vec3 input(0.0f);

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
		input -= m_RightDirection;
	}
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
	{
		input += m_RightDirection;
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

	if (input.x != 0 || input.y != 0 || input.z != 0)
	{
		m_Position += glm::normalize(input) * m_Window.GetDeltaTime() * mulitplier * m_Speed;

		m_Moved = true;
	}

	if (m_Moved)
	{
		RecalculateView();
	}

	if (m_Window.SizeChanged())
	{
		RecalculateProjection();
	}

	if (m_FOVChanged)
	{
		m_Moved = true;
		m_FOVChanged = false;
	}
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

bool Camera::HasMoved() const
{
	return m_Moved;
}

void Camera::SetFOV(float fov)
{
	m_VerticalFOV = fov;
	m_FOVChanged = true;

	RecalculateProjection();
}

void Camera::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Camera::SetSens(float sens)
{
	m_Sensitivity = sens;
}

void Camera::RecalculateVectors()
{
	float sin_yaw = sinf(m_Rotation.y);
	float cos_yaw = cosf(m_Rotation.y);
	float sin_pitch = sinf(m_Rotation.x);
	float cos_pitch = cosf(m_Rotation.x);

	m_ForwardDirection = glm::vec3(sin_yaw * cos_pitch, sin_pitch, cos_yaw * cos_pitch);
	m_RightDirection = glm::normalize(glm::cross(UP_DIR, m_ForwardDirection));
	m_UpDirection = glm::normalize(glm::cross(m_ForwardDirection, m_RightDirection));
}

void Camera::RecalculateProjection()
{
	glm::mat4x4 perspective = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_Window.GetWindowWidth(), (float)m_Window.GetWindowHeight(), m_NearClip, m_FarClip);

	m_InverseProjection = glm::inverse(perspective);
}

void Camera::RecalculateView()
{
	glm::mat4x4 view = glm::lookAt(m_Position, m_Position + m_ForwardDirection, m_UpDirection);

	m_InverseView = glm::inverse(view);
}