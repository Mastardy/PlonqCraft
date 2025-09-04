#include <chrono>
#include <print>

#define GLM_ENABLE_EXPERIMENTAL

#include "Camera/Camera.hpp"
#include "glad/glad.h"
#include "Graphics/Shader.hpp"
#include "Graphics/SSBO.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/TimeQuery.hpp"
#include "Graphics/Window.hpp"
#include "Input/Input.hpp"
#include "Random/Random.hpp"
#include "Voxel/Block.hpp"
#include "Voxel/Chunk.hpp"

void ProcessInput(const Window& window, Camera& camera);

#define CHUNK_WIDTH 16
#define CHUNK_LENGTH 16
#define CHUNK_HEIGHT 256

int main()
{
	Window window(640, 640);
	Camera camera(glm::vec3(0, 0, 0), 0, 0);

	Input(window.GetGLFWWindow());

	const auto texWidth = window.GetWidth();
	const auto texHeight = window.GetHeight();
	const auto texture = window.GetTexture();
	const auto computeShader = std::make_shared<Shader>("res/shaders/compute/raytracer.comp");

	Chunk chunk(CHUNK_WIDTH, CHUNK_LENGTH, CHUNK_HEIGHT);

	for (auto y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (auto z = 0; z < CHUNK_LENGTH; z++)
		{
			for (auto x = 0; x < CHUNK_WIDTH; x++)
			{
				const bool chess = (x + y % 2 + z % 2) % 2 == 0;
				chunk.SetBlock(x, y, z, Block(chess ? BlockType::Air : BlockType::Stone));
			}
		}
	}

	TimeQuery timeQuery;
	SSBO chunkBO(chunk.GetVector(), 0, GL_DYNAMIC_DRAW);

	Random::Seed(time(nullptr));
	const auto randSeed = Random::GetValue(0, FLT_MAX);

	computeShader->Use();

	computeShader->SetFloat("camFocalLength", 1.0f);
	computeShader->SetInt("texWidth", texWidth);
	computeShader->SetInt("texHeight", texHeight);
	computeShader->SetFloat("randSeed", randSeed);
	computeShader->SetInt("chunkWidth", CHUNK_WIDTH);
	computeShader->SetInt("chunkLength", CHUNK_LENGTH);
	computeShader->SetInt("chunkHeight", CHUNK_HEIGHT);

	auto titleUpdater = std::chrono::high_resolution_clock::now();
	while (!window.ShouldClose())
	{
		timeQuery.Begin();

		computeShader->Use();
		computeShader->SetVec3("camCenter", camera.GetPosition());
		computeShader->SetVec3("camForward", camera.GetForward());

		texture->Clear();
		glDispatchCompute(static_cast<unsigned int>(texWidth) / 8, static_cast<unsigned int>(texHeight) / 8, 1);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		window.Render();

		Input::Process();
		ProcessInput(window, camera);

		timeQuery.GetQuery();
		auto end = std::chrono::high_resolution_clock::now();
		if (auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - titleUpdater);
			duration.count() < 1000)
			continue;

		titleUpdater = end;
		window.SetTitle(std::format("Frame Time: {:^4}ms, FPS: {:.0f}", timeQuery.FrameTime(),
		                            1.0f / timeQuery.DeltaTime()));
	}

	return 0;
}

void ProcessInput(const Window& window, Camera& camera)
{
	if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) window.Close();

	if (Input::GetKeyDown(GLFW_KEY_W)) camera.Move(0, 0, 1);
	if (Input::GetKeyDown(GLFW_KEY_S)) camera.Move(0, 0, -1);
	if (Input::GetKeyDown(GLFW_KEY_A)) camera.Move(-1, 0, 0);
	if (Input::GetKeyDown(GLFW_KEY_D)) camera.Move(1, 0, 0);
	if (Input::GetKeyDown(GLFW_KEY_SPACE)) camera.Move(0, 1, 0);
	if (Input::GetKeyDown(GLFW_KEY_LEFT_CONTROL)) camera.Move(0, -1, 0);

	if (Input::GetKeyDown(GLFW_KEY_LEFT)) camera.Rotate(0, -30);
	if (Input::GetKeyDown(GLFW_KEY_RIGHT)) camera.Rotate(0, 30);
	if (Input::GetKeyDown(GLFW_KEY_UP)) camera.Rotate(30, 0);
	if (Input::GetKeyDown(GLFW_KEY_DOWN)) camera.Rotate(-30, 0);
}
