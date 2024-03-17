#pragma once

#include "Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(const Window& window, float verticalFOV, float near, float far, float speed, float sensitivity);

	void Update();

	const glm::vec3& GetPosition() const;
	const glm::mat4x4& GetInverseProjectionMatrix() const;
	const glm::mat4x4& GetInverseViewMatrix() const;

private:
	const Window& m_Window;
	float m_VerticalFOV;
	float m_NearClip;
	float m_FarClip;
	float m_Speed;
	float m_Sensitivity;

	glm::mat4x4 m_InverseProjection { 1.0f };
	glm::mat4x4 m_InverseView { 1.0f };
	glm::vec3 m_Position { 0.0f, 0.0f, -5.0f };
	glm::vec2 m_Rotation { 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection { 0.0f, 0.0f, 1.0f };
	glm::vec3 m_UpDirection { 0.0f, 1.0f, 0.0f };
	glm::vec2 m_LastMousePosition { 0.0f, 0.0f };
	bool m_FirstCameraClick { false };

	void TryRecalculateProjection();
	void RecalculateView();
};