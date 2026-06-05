#include "Core/Config.h"
#include "Core/Logger.h"
#include "Platform/Input.h"
#include "Platform/Window.h"
#include "Platform/Time.h"
#include "Scene/Camera/FlyCameraController.h"
#include "Scene/CornellBox.h"
#include "Renderer/RenderPipeline.h"
#include "Renderer/RenderUtils/GPUScene.h"
#include "GUI/GUI.h"
#include "GUI/StatsWindow.h"
#include "GUI/ConfigWindow.h"

struct Settings
{
	WindowConfig windowConfig;
	RayTracingConfig rayTracingConfig;
	BloomConfig bloomConfig;
	ToneMapConfig toneMapConfig;
	BVHDebugConfig bvhDebugConfig;
	FlyCameraSettings cameraSettings;
	CameraProjectionSettings cameraProjectionSettings;
};

int main()
{
	// Setup logging
	spdlog::set_level(spdlog::level::trace);

	// Settings setup
	Settings settings{};
	settings.windowConfig = { false };
	settings.rayTracingConfig = { true };
	settings.bloomConfig = { true, 5, 0.005f, 0.05f };
	settings.toneMapConfig = { 1.0f, 2.2f };
	settings.bvhDebugConfig = { false };
	settings.cameraSettings = { 7.0f, 3.5f };
	settings.cameraProjectionSettings = { 70.0f, 0.3f, 1000.0f };

	// Platform setup
	Window window(1920, 1080, "Ray Tracing", settings.windowConfig);
	Input input(window);
	Time time;

	// Scene setup
	std::unique_ptr<FlyCameraController> cameraController = std::make_unique<FlyCameraController>(input, settings.cameraSettings);
	CornellBox exampleSceneMaker(std::move(cameraController));
	Scene& scene = exampleSceneMaker.GetScene();
	Camera& camera = scene.GetCamera();	

	// Renderer setup
	GPUScene gpuScene(scene);
	RenderPipeline renderPipeline;
	RenderViewport renderViewport{};
	RenderContext renderContext{ scene.GetCamera(), renderViewport, gpuScene };

	// GUI setup
	GUI gui(window);
	gui.AddWindow(std::make_unique<StatsWindow>(time));
	gui.AddWindow(std::make_unique<ConfigWindow>(settings.windowConfig, settings.rayTracingConfig, settings.bloomConfig, settings.toneMapConfig, settings.cameraSettings, settings.cameraProjectionSettings));

	// Main loop
	while (window.ShouldUpdate())
	{
		// Update Platform
		window.PollEvents();
		input.NewFrame();
		time.Update();

		// Update Scene
		scene.Update(time.GetDeltaSeconds());

		// Update render context
		renderViewport.width = window.GetWindowWidth();
		renderViewport.height = window.GetWindowHeight();

		if (renderViewport.width > 0 && renderViewport.height > 0)   // avoid rendering when minimized
		{
			// Update camera
			camera.SetAspectRatio((float)renderViewport.width / (float)renderViewport.height);
			camera.SetFOV(settings.cameraProjectionSettings.fov);
			camera.SetNearPlane(settings.cameraProjectionSettings.nearPlane);
			camera.SetFarPlane(settings.cameraProjectionSettings.farPlane);

			// Handle input
			if (input.WasKeyPressed(KeyCode::R))
			{
				renderPipeline.Reload();
			}
			if (input.WasKeyPressed(KeyCode::B))
			{
				settings.bvhDebugConfig.enabled = !settings.bvhDebugConfig.enabled;
			}

			// Render
			renderPipeline.Execute(renderContext, settings.rayTracingConfig, settings.bloomConfig, settings.toneMapConfig, settings.bvhDebugConfig);
		}

		// Update GUI
		gui.Update();

		// Swap buffers
		window.SwapBuffers();
	}
}