#pragma once

#include <array>
#include <glm/glm.hpp>
#include "Window.h"
#include "KeyCode.h"

class Input
{
public:
	explicit Input(Window& window);

	// Called ONCE per frame by Application, AFTER glfwPollEvents, BEFORE any subsystem Update().
	void NewFrame();

	// Keyboard
	bool IsKeyDown(KeyCode key) const;
	bool WasKeyPressed(KeyCode key) const;
	bool WasKeyReleased(KeyCode key) const;

	// Mouse buttons
	bool IsMouseDown(MouseButton button) const;
	bool WasMousePressed(MouseButton button) const;

	// Mouse motion
	glm::vec2 MousePosition() const;
	glm::vec2 MouseDelta() const;   // pixels since last frame
	glm::vec2 MouseDeltaNormalized() const;   // delta normalized by window size, centered at (0, 0) and with y flipped (so up is positive)

	// Cursor capture (FPS-style look). Resets delta so capture/release doesn't cause a jump.
	void SetCursorCaptured(bool captured);
	bool IsCursorCaptured() const;

private:
	Window& m_Window;

	static constexpr int KeyCount = 349;  // GLFW_KEY_LAST (348) + 1
	static constexpr int ButtonCount = 8;    // GLFW_MOUSE_BUTTON_LAST (7) + 1

	std::array<bool, KeyCount> m_KeysNow{}, m_KeysPrev{};
	std::array<bool, ButtonCount> m_ButtonsNow{}, m_ButtonsPrev{};

	glm::vec2 m_MousePos{ 0.0f };
	glm::vec2 m_MousePosNormalized{ 0.0f };
	glm::vec2 m_MouseDelta{ 0.0f };
	glm::vec2 m_MouseDeltaNormalized{ 0.0f };
	bool m_CursorCaptured = false;
	bool m_SkipNextDelta = true;   // suppress the first/post-capture jump
};