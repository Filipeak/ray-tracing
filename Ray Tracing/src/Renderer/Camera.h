#pragma once

#include "Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
 * TODO: Change to Left Handed Coordinate System
 * - https://learnopengl.com/Getting-started/Coordinate-Systems
 * - https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/coordinate-systems.html
 */

class Camera
{
public:
	Camera(const Window& window, glm::vec3 startPosition, glm::vec2 startRotation, float verticalFOV, float near, float far, float speed, float sensitivity);

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
	glm::vec3 m_Position { 0.0f, 0.0f, 0.0f };
	glm::vec2 m_Rotation { 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_UpDirection { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_RightDirection { 0.0f, 0.0f, 0.0f };
	glm::vec2 m_LastMousePosition { 0.0f, 0.0f };
	bool m_FirstCameraClick { false };

	void RecalculateForwardVector();
	void RecalculateRightAndUpVectors();
	void RecalculateProjectionIfAvailable();
	void RecalculateView();
};