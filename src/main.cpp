#include <print>

#include "Graphics/Shader.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Primitives/Shape.hpp"

void ProcessInput(const Window& window);

int main()
{
	Window window;

	constexpr float magicNumber = 1.f;

	auto shader = std::make_shared<Shader>("res/shaders/base/base.vert", "res/shaders/base/base.frag");
	auto mat = std::make_shared<Material>(shader);

	constexpr auto vertices = {
		glm::vec3(-magicNumber, -magicNumber, magicNumber),
		glm::vec3(-magicNumber, magicNumber, magicNumber),
		glm::vec3(magicNumber, -magicNumber, magicNumber),
		glm::vec3(magicNumber, magicNumber, magicNumber),
		glm::vec3(-magicNumber, -magicNumber, -magicNumber),
		glm::vec3(-magicNumber, magicNumber, -magicNumber),
		glm::vec3(magicNumber, -magicNumber, -magicNumber),
		glm::vec3(magicNumber, magicNumber, -magicNumber)
	};

	for (auto x = -2; x <= 2; x++)
	{
		for (auto y = -2; y <= 2; y++)
		{
			for (auto z = 0; z < 5; z++)
			{
				constexpr auto indices = {
					0, 1, 2,
					2, 1, 3,
					4, 0, 6,
					6, 0, 2,
					5, 1, 4,
					4, 1, 0,
					7, 3, 5,
					5, 3, 1,
					6, 2, 7,
					7, 2, 3,
					6, 7, 4,
					4, 7, 5
				};
				window.AddShape(std::move(std::make_unique<Shape>(vertices, indices, glm::vec3(x * magicNumber, y * magicNumber, -z * magicNumber), mat)));
			}
		}
	}

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
