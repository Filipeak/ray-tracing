#define GLM_FORCE_LEFT_HANDED

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

static constexpr glm::vec3 UP_DIR = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(glm::vec3 startPosition, glm::vec2 startRotation) : m_FOV(70.0f), m_AspectRatio(16.0f / 9.0f), m_NearPlane(0.3f), m_FarPlane(1000.0f)
{
	SetPosition(startPosition);
	SetRotation(startRotation);

	RecalculateProjection();
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_Position = position;

	RecalculateView();
}

void Camera::SetRotation(const glm::vec2& rotation)
{
	m_Rotation = rotation;

	RecalculateBasis();
	RecalculateView();
}

void Camera::SetFOV(float fov)
{
	if (m_FOV != fov)
	{
		m_FOV = fov;
		RecalculateProjection();
	}
}

void Camera::SetAspectRatio(float aspectRatio)
{
	if (m_AspectRatio != aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		RecalculateProjection();
	}
}

void Camera::SetNearPlane(float nearPlane)
{
	if (m_NearPlane != nearPlane)
	{
		m_NearPlane = nearPlane;
		RecalculateProjection();
	}
}

void Camera::SetFarPlane(float farPlane)
{
	if (m_FarPlane != farPlane)
	{
		m_FarPlane = farPlane;
		RecalculateProjection();
	}
}

const glm::vec3& Camera::GetPosition() const
{
	return m_Position;
}

const glm::vec2& Camera::GetRotation() const
{
	return m_Rotation;
}

const glm::vec3& Camera::GetForwardDirection() const
{
	return m_ForwardDirection;
}

const glm::vec3& Camera::GetUpDirection() const
{
	return m_UpDirection;
}

const glm::vec3& Camera::GetLeftDirection() const
{
	return m_LeftDirection;
}

const glm::mat4x4& Camera::GetViewMatrix() const
{
	return m_View;
}

const glm::mat4x4& Camera::GetInverseViewMatrix() const
{
	return m_InverseView;
}

const glm::mat4x4& Camera::GetProjectionMatrix() const
{
	return m_Projection;
}

const glm::mat4x4& Camera::GetInverseProjectionMatrix() const
{
	return m_InverseProjection;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
	m_InverseProjection = glm::inverse(m_Projection);
}

void Camera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, m_UpDirection);
	m_InverseView = glm::inverse(m_View);
}

void Camera::RecalculateBasis()
{
	float sin_yaw = sinf(m_Rotation.y);
	float cos_yaw = cosf(m_Rotation.y);
	float sin_pitch = sinf(m_Rotation.x);
	float cos_pitch = cosf(m_Rotation.x);

	m_ForwardDirection = glm::vec3(sin_yaw * cos_pitch, sin_pitch, cos_yaw * cos_pitch);
	m_LeftDirection = glm::normalize(glm::cross(UP_DIR, m_ForwardDirection));
	m_UpDirection = glm::normalize(glm::cross(m_ForwardDirection, m_LeftDirection));
}