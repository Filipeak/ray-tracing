#include "RTRenderer.h"
#include "GLLog.h"
#include "../Core/Config.h"

RTRenderer::RTRenderer(const Window& window) : m_Window(window), m_ReloadKeyPressTest(false)
{
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		+1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, +1.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	m_VertexArray = new VertexArray();
	m_VertexBuffer = new VertexBuffer(vertices, sizeof(vertices) / sizeof(float));
	m_IndexBuffer = new IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

	m_VertexArray->PushLayoutFloat(3);
	m_VertexArray->PushLayoutFloat(2);
	m_VertexArray->SaveLayout();

	m_Shader = new Shader("shaders/quad.vert", "shaders/rt.frag");
	m_ScreenShader = new Shader("shaders/quad.vert", "shaders/pp.frag");

	m_Camera = new Camera(window, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec2(0.0f, glm::pi<float>()), CAMERA_FOV, 0.3f, 1000.0f, CAMERA_SPEED, CAMERA_SENS);

	m_Framebuffer = new Framebuffer(window);

	m_StartTime = std::chrono::high_resolution_clock::now();
}

RTRenderer::~RTRenderer()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_IndexBuffer;
	delete m_Shader;
	delete m_ScreenShader;
	delete m_Camera;
	delete m_Framebuffer;
}

void RTRenderer::Update()
{
	if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_R) == GLFW_PRESS)
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

	m_Camera->Update();
}

/*
 * TODO: Progressive Rendering / Accumulation
 * - https://www.youtube.com/watch?v=46ddlUImiQA&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=10
 * - https://www.youtube.com/watch?v=A61S_2swwAc
 * - https://www.youtube.com/watch?v=QQ3jr-9Rc1o
 * - https://learnopengl.com/Advanced-OpenGL/Framebuffers
 * - https://github.com/carl-vbn/opengl-raytracing/blob/main/src/main.cpp
 * - https://github.com/Eduard0110/Path-tracer-using-OpenGL/blob/main/source/Main.cpp
 * - https://github.com/ishaanshah/raytracer/blob/main/src/main.cpp#L181
 * - https://stackoverflow.com/questions/23990366/accumulation-buffer-using-glfw
 * - https://uysalaltas.github.io/2022/01/09/OpenGL_Imgui.html
 * 
 * TODO: Gamma Correction
 * - https://learnopengl.com/Advanced-Lighting/Gamma-Correction
 * - https://en.wikipedia.org/wiki/Gamma_correction
 * - https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/usinggammacorrectionforaccuratecolorintensity
 * - https://www.youtube.com/watch?v=wFx0d9c8WMs
 * - https://www.youtube.com/watch?v=op805g4SZgE
 * 
 * TODO: Bloom & HDR
 * - https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model#Fragment_shader
 * - https://learnopengl.com/Advanced-Lighting/Bloom
 * - https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
 * - https://www.youtube.com/watch?v=tI70-HIc5ro
 * - https://learnopengl.com/Advanced-Lighting/HDR
 * - https://www.youtube.com/watch?v=1U-jym5WADU
 * - https://www.youtube.com/watch?v=iikdcAA7cww
 */
void RTRenderer::Render()
{
	m_Framebuffer->Bind();

	std::chrono::duration<float> fsec = std::chrono::high_resolution_clock::now() - m_StartTime;

	m_Shader->Bind();
	m_Shader->SetVec2("u_Resolution", glm::vec2(m_Window.GetViewportWidth(), m_Window.GetViewportHeight()));
	m_Shader->SetFloat("u_Time", fsec.count());
	m_Shader->SetVec3("u_CameraPosition", m_Camera->GetPosition());
	m_Shader->SetMat4x4("u_CameraInverseProjection", m_Camera->GetInverseProjectionMatrix());
	m_Shader->SetMat4x4("u_CameraInverseView", m_Camera->GetInverseViewMatrix());

	m_VertexArray->Bind();

	OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	OPENGL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


	m_Framebuffer->Unbind();

	m_ScreenShader->Bind();
	m_Framebuffer->BindTexture();

	m_VertexArray->Bind();

	OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	OPENGL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}