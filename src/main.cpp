#include <print>

#include "glad/glad.h"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Primitives/Shape.hpp"

void ProcessInput(const Window& window);

int main()
{
	Window window(400, 400);

	auto shader = std::make_shared<Shader>("res/shaders/base/base.vert", "res/shaders/base/base.frag");
	const auto texWidth = window.GetWidth();
	const auto texHeight = window.GetHeight();
	auto texture = std::make_shared<Texture>(texWidth, texHeight);
	auto mat = std::make_shared<Material>(shader, texture);

	const auto computeShader = std::make_shared<Shader>("res/shaders/compute/test.comp");

	constexpr auto vertices = {
		VertexData{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
		VertexData{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		VertexData{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
		VertexData{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
	};

	window.AddShape(std::move(std::make_unique<Shape>(vertices, mat)));

#pragma region Ray Tracing

	// Camera
	constexpr auto cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);

#pragma endregion

	while (!window.ShouldClose())
	{
		computeShader->Use();

		computeShader->SetVec3("camCenter", cameraCenter);
		computeShader->SetFloat("camFocalLength", 1.0f);
		computeShader->SetInt("texWidth", texWidth);
		computeShader->SetInt("texHeight", texHeight);
		computeShader->SetFloat("t", static_cast<float>(glfwGetTime()));

		glDispatchCompute(static_cast<unsigned int>(texWidth) / 8, static_cast<unsigned int>(texHeight) / 8, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		window.Render();
		ProcessInput(window);
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
