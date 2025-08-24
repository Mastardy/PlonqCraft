#include "Graphics/Window.hpp"
#include "Graphics/Primitives/Shape.hpp"

void ProcessInput(const Window& window);

int main()
{
	Window window;

	Material mat;
	const Shape triangle(
	{
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f)
	}, {0, 1, 2, 1, 3, 2}, mat);

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
