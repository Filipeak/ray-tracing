#pragma once

#include <chrono>
#include "Window.h"
#include "Camera.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class RTRenderer
{
public:
	RTRenderer(const Window& window);
	~RTRenderer();

	void Update();
	void Render();

private:
	std::chrono::steady_clock::time_point m_StartTime;
	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;
	Shader* m_Shader;
	const Window& m_Window;
	bool m_ReloadKeyPressTest;
	Camera m_Camera;
};