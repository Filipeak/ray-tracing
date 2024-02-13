#include "../Renderer/Window.h"
#include "../Renderer/RTRenderer.h"
#include "../GUI/GUI.h"

int main()
{
	Window window;
	RTRenderer renderer(window);
	GUI gui(window);

	while (window.ShouldUpdate())
	{
		gui.Begin();
		renderer.Update();
		renderer.Render();
		gui.End();

		window.SwapBuffers();
	}
}