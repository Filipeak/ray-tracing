#include "FlyCameraController.h"
#include <glm/gtc/constants.hpp>

static constexpr float MAX_PITCH_RAD = glm::pi<float>() / 3.0f;

FlyCameraController::FlyCameraController(Input& input, const FlyCameraSettings& settings) : m_Input(input), m_Settings(settings)
{
}

/*
 * REF: https://learnopengl.com/Getting-started/Camera
 * REF: https://learnopengl.com/Getting-started/Coordinate-Systems
 * REF: https://www.youtube.com/watch?v=lXlXqUEEJ94&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=7
 */
void FlyCameraController::Update(Camera& camera, float dt)
{
	if (m_Input.IsMouseDown(MouseButton::Right))
	{
		m_Input.SetCursorCaptured(true);

		glm::vec2 mouseDelta = m_Input.MouseDeltaNormalized();
		float deltaPitch = m_Settings.sensitivity * mouseDelta.y;
		float deltaYaw = m_Settings.sensitivity * mouseDelta.x;

		if (deltaPitch != 0 || deltaYaw != 0)
		{
			glm::vec2 rotation = camera.GetRotation();
			rotation.x += deltaPitch;
			rotation.y += deltaYaw;
			rotation.x = glm::clamp(rotation.x, -MAX_PITCH_RAD, MAX_PITCH_RAD);

			camera.SetRotation(rotation);
		}
	}
	else
	{
		m_Input.SetCursorCaptured(false);
	}

	float multiplier = 1.0f;
	glm::vec3 input(0.0f);

	if (m_Input.IsKeyDown(KeyCode::W))
	{
		input += camera.GetForwardDirection();
	}
	if (m_Input.IsKeyDown(KeyCode::S))
	{
		input -= camera.GetForwardDirection();
	}
	if (m_Input.IsKeyDown(KeyCode::A))
	{
		input -= camera.GetLeftDirection();
	}
	if (m_Input.IsKeyDown(KeyCode::D))
	{
		input += camera.GetLeftDirection();
	}
	if (m_Input.IsKeyDown(KeyCode::Q))
	{
		input -= camera.GetUpDirection();
	}
	if (m_Input.IsKeyDown(KeyCode::E))
	{
		input += camera.GetUpDirection();
	}
	if (m_Input.IsKeyDown(KeyCode::LeftShift))
	{
		multiplier = 2.0f;
	}

	if (input.x != 0 || input.y != 0 || input.z != 0)
	{
		glm::vec3 position = camera.GetPosition() + glm::normalize(input) * dt * multiplier * m_Settings.speed;
		camera.SetPosition(position);
	}
}