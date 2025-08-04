#include "Graphics/Window.hpp"
#include "Graphics/Primitives/Triangle.hpp"

void ProcessInput(const Window& window);

int main()
{
	const Window window;

	const Triangle triangle;

	while (!window.ShouldClose())
	{
		ProcessInput(window);
		window.Render();
	}

	return 0;
}

void ProcessInput(const Window& window)
{
	if (window.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		window.ShouldClose(true);
	}
}
