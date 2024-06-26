#pragma once

#include <chrono>
#include "Window.h"
#include "Camera.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Framebuffer.h"

class RTRenderer
{
public:
	RTRenderer(const Window& window);
	~RTRenderer();

	void Update();
	void Render();

private:
	const Window& m_Window;
	Framebuffer* m_Framebuffer;

	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;

	Shader* m_Shader;
	Shader* m_ScreenShader;

	bool m_ReloadKeyPressTest;
	Camera* m_Camera;
	std::chrono::steady_clock::time_point m_StartTime;
};