#include "Input.h"
#include <GLFW/glfw3.h>

Input::Input(Window& window) : m_Window(window)
{
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(window.GetWindowHandle(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

void Input::NewFrame()
{
	m_KeysPrev = m_KeysNow;
	m_ButtonsPrev = m_ButtonsNow;

	GLFWwindow* w = m_Window.GetWindowHandle();

	for (int k = GLFW_KEY_SPACE; k <= GLFW_KEY_LAST; ++k)
	{
		m_KeysNow[k] = (glfwGetKey(w, k) == GLFW_PRESS);
	}

	for (int b = 0; b <= GLFW_MOUSE_BUTTON_LAST; ++b)
	{
		m_ButtonsNow[b] = (glfwGetMouseButton(w, b) == GLFW_PRESS);
	}

	double mx, my;
	glfwGetCursorPos(w, &mx, &my);
	glm::vec2 pos((float)mx, (float)my);
	glm::vec2 posNormalized((pos.x - m_Window.GetWindowWidth() / 2.0f) / m_Window.GetWindowWidth(), (m_Window.GetWindowHeight() / 2.0f - pos.y) / m_Window.GetWindowHeight()); // y is inversed
	m_MouseDelta = m_SkipNextDelta ? glm::vec2(0.0f) : (pos - m_MousePos);
	m_MouseDeltaNormalized = m_SkipNextDelta ? glm::vec2(0.0f) : (posNormalized - m_MousePosNormalized);
	m_MousePos = pos;
	m_MousePosNormalized = posNormalized;
	m_SkipNextDelta = false;
}

bool Input::IsKeyDown(KeyCode key) const
{
	return m_KeysNow[(int)key];
}

bool Input::WasKeyPressed(KeyCode key) const
{
	return m_KeysNow[(int)key] && !m_KeysPrev[(int)key];
}

bool Input::WasKeyReleased(KeyCode key) const
{
	return !m_KeysNow[(int)key] && m_KeysPrev[(int)key];
}

bool Input::IsMouseDown(MouseButton b) const
{
	return m_ButtonsNow[(int)b];
}

bool Input::WasMousePressed(MouseButton b) const
{
	return m_ButtonsNow[(int)b] && !m_ButtonsPrev[(int)b];
}

glm::vec2 Input::MousePosition() const
{
	return m_MousePos;
}

glm::vec2 Input::MouseDelta() const
{
	return m_MouseDelta;
}

glm::vec2 Input::MouseDeltaNormalized() const
{
	return m_MouseDeltaNormalized;
}

void Input::SetCursorCaptured(bool captured)
{
	if (captured == m_CursorCaptured)
	{
		return;
	}

	m_CursorCaptured = captured;

	glfwSetInputMode(m_Window.GetWindowHandle(), GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

	m_SkipNextDelta = true;
}

bool Input::IsCursorCaptured() const
{
	return m_CursorCaptured;
}