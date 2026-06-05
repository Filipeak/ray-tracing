#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec2 startRotation);

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec2& rotation);

	void SetFOV(float fov);
	void SetAspectRatio(float aspectRatio);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	const glm::vec3& GetPosition() const;
	const glm::vec2& GetRotation() const;
	const glm::vec3& GetForwardDirection() const;
	const glm::vec3& GetUpDirection() const;
	const glm::vec3& GetLeftDirection() const;

	const glm::mat4x4& GetViewMatrix() const;
	const glm::mat4x4& GetInverseViewMatrix() const;
	const glm::mat4x4& GetProjectionMatrix() const;
	const glm::mat4x4& GetInverseProjectionMatrix() const;

private:
	float m_FOV;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;

	glm::mat4x4 m_Projection{ 1.0f };
	glm::mat4x4 m_View{ 1.0f };
	glm::mat4x4 m_InverseProjection{ 1.0f };
	glm::mat4x4 m_InverseView{ 1.0f };
	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	glm::vec2 m_Rotation{ 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_UpDirection{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_LeftDirection{ 0.0f, 0.0f, 0.0f };

	void RecalculateProjection();
	void RecalculateView();
	void RecalculateBasis();
};