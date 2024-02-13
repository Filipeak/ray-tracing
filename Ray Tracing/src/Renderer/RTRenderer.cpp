#include "RTRenderer.h"
#include "GLLog.h"

RTRenderer::RTRenderer(const Window& window) : m_Window(window), m_ReloadKeyPressTest(false), m_Camera()
{
	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	m_VertexArray = new VertexArray();
	m_VertexBuffer = new VertexBuffer(vertices, sizeof(vertices) / sizeof(float));
	m_IndexBuffer = new IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

	m_VertexArray->PushLayoutFloat(3);
	m_VertexArray->SaveLayout();

	m_Shader = new Shader("shaders/rt.vert", "shaders/rt.frag");
	m_Shader->Bind();

	m_StartTime = std::chrono::high_resolution_clock::now();
}

RTRenderer::~RTRenderer()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_IndexBuffer;
	delete m_Shader;
}

void RTRenderer::Update()
{
	if (glfwGetKey(m_Window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS)
	{
		if (!m_ReloadKeyPressTest)
		{
			m_Shader->Reload();

			m_ReloadKeyPressTest = true;
		}
	}
	else
	{
		m_ReloadKeyPressTest = false;
	}

	m_Camera.Update(m_Window);
}

void RTRenderer::Render()
{
	std::chrono::duration<float> fsec = std::chrono::high_resolution_clock::now() - m_StartTime;

	m_Shader->SetUniform2f("u_Resolution", (float)m_Window.GetWidth(), (float)m_Window.GetHeight());
	m_Shader->SetUniform1f("u_Time", fsec.count());
	m_Shader->SetUniform3f("u_CameraPos", m_Camera.GetPos().x, m_Camera.GetPos().y, m_Camera.GetPos().z);

	OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	OPENGL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}