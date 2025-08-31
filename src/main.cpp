#include <chrono>
#include <print>

#include "glad/glad.h"
#include "glm/gtc/noise.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Primitives/Shape.hpp"

class Camera
{
public:
	glm::vec3 position;
};

struct Sphere
{
	glm::vec3 center;
	float radius;
};

void ProcessInput(const Window& window, Camera& camera, double deltaTime);

int main()
{
	Window window(800, 800);

	auto shader = std::make_shared<Shader>("res/shaders/base/base.vert", "res/shaders/base/base.frag");
	const auto texWidth = window.GetWidth();
	const auto texHeight = window.GetHeight();
	auto texture = std::make_shared<Texture>(texWidth, texHeight);
	auto mat = std::make_shared<Material>(shader, texture);

	const auto computeShader = std::make_shared<Shader>("res/shaders/compute/raytracer.comp");

	constexpr auto vertices = {
		VertexData{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
		VertexData{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		VertexData{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
		VertexData{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
	};

	window.AddShape(std::move(std::make_unique<Shape>(vertices, mat)));

#pragma region Ray Tracing

	// Camera
	auto camera = Camera();

	const std::vector<Sphere> spheres = {
		{{0, -100.5, -1}, 100},
		{{0.0f, 0.0f, -1.2f}, 0.5f},
		{{1.0f, 0.0f, -1.0f}, 0.5f},
		{{-1.0f, 0.0f, -1.0f}, 0.5f}
	};

#pragma endregion

	unsigned int query;
	glGenQueries(1, &query);
	unsigned int ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<long long>(spheres.size() * sizeof(Sphere)), spheres.data(),
	             GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	auto titleUpdater = std::chrono::high_resolution_clock::now();
	while (!window.ShouldClose())
	{
		glBeginQuery(GL_TIME_ELAPSED, query);

		computeShader->Use();

		computeShader->SetVec3("camCenter", camera.position);
		computeShader->SetFloat("camFocalLength", 1.0f);
		computeShader->SetInt("texWidth", texWidth);
		computeShader->SetInt("texHeight", texHeight);

		texture->Clear();
		glDispatchCompute(static_cast<unsigned int>(texWidth) / 32, static_cast<unsigned int>(texHeight) / 32, 1);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		window.Render();

		glEndQuery(GL_TIME_ELAPSED);

		int available = 0;
		while (!available)
			glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &available);
		unsigned long long timeElapsed;
		glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);
		long double deltaTime = static_cast<double>(timeElapsed) / 1e6;
		ProcessInput(window, camera, deltaTime);

		auto end = std::chrono::high_resolution_clock::now();
		if (auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - titleUpdater);
			duration.count() <= 1000)
			continue;
		titleUpdater = end;
		window.SetTitle(std::format("Frame Time: {:^4}ms, FPS: {:.0f}", deltaTime, 1000.0f / deltaTime));
	}

	glDeleteQueries(1, &query);
	glDeleteBuffers(1, &ssbo);
	return 0;
}

void ProcessInput(const Window& window, Camera& camera, double deltaTime)
{
	if (window.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		window.Close();
	}

	if (window.GetKey(GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.position.z -= static_cast<float>(deltaTime) * 0.1f;
	}
	if (window.GetKey(GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.position.z += static_cast<float>(deltaTime) * 0.1f;
	}
	if (window.GetKey(GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.position.x -= static_cast<float>(deltaTime) * 0.1f;
	}
	if (window.GetKey(GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.position.x += static_cast<float>(deltaTime) * 0.1f;
	}
}
