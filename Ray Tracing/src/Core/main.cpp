#include "Config.h"
#include "../Renderer/Window.h"
#include "../Renderer/RTRenderer.h"
#include "../GUI/GUI.h"
#include "../GUI/StatsWindow.h"
#include "../GUI/ConfigWindow.h"

int main()
{
	ConfigData config;

	Window window(1920, 1080, "Ray Tracing", config.vsync);
	Camera camera(window, glm::vec3(0.0f, 0.0f, -10.0f), glm::vec2(0.0f, 0.0f), config.cameraFOV, 0.3f, 1000.0f, config.cameraSpeed, config.cameraSens);
	RTRenderer renderer(window, camera, config.accumulate, config.hdrExposure, config.screenGamma, config.bloom, config.bloomStrength, config.bloomFilterRadius);
	GUI gui(window);
	gui.AddWindow(new StatsWindow());
	gui.AddWindow(new ConfigWindow(config, window, renderer, camera));

	while (window.ShouldUpdate())
	{
		gui.Begin();
		renderer.Update();
		gui.End();

		window.SwapBuffers();
	}
}