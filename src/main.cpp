#include "Graphics/Window.hpp"
#include "Graphics/Primitives/Triangle.hpp"

void ProcessInput(const Window& window);

int main()
{
	Window window;

	Material mat;
	const Triangle triangle(
	{
		glm::vec3(0.0, 0.5, 0.0),
		glm::vec3(-0.5, -0.5, 0.0),
		glm::vec3(0.5, -0.5, 0.0)
	}, mat);

	window.AddTriangle(triangle);

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
		window.Close();
	}
}
