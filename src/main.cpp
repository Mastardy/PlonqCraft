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

void ProcessInput(const Window& window, Camera& camera, float deltaTime);

void WorkChunk(Chunk& chunk, const int x, const int y, const int z)
{
	// chunk.SetBlock(x, y, z, Block(BlockType::Stone));
	const bool chess = (x + y % 2 + z % 2) % 2 == 0;
	chunk.SetBlock(x, y, z, Block(!chess ? BlockType::Air : BlockType::Stone));
}

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

	Chunk mainChunk(CHUNK_WIDTH, CHUNK_LENGTH, CHUNK_HEIGHT);

	std::function worker = WorkChunk;

	mainChunk.DoWork(worker);

	TimeQuery timeQuery;
	SSBO chunkBO(mainChunk.GetVector(), 0, GL_DYNAMIC_DRAW);

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

	constexpr float theta = glm::radians(120.0f);
	constexpr float phi = glm::radians(30.0f);
	const auto sunDir = glm::vec3(-cos(theta) * -sin(phi), -cos(phi), -sin(theta) * -sin(phi));

	computeShader->SetVec3("sunLights", sunDir);

	long double titleUpdater = 0;
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

		ProcessInput(window, camera, 0.0001f);

		timeQuery.GetQuery();
		titleUpdater += timeQuery.DeltaTime();
		if (titleUpdater < 1000) continue;
		titleUpdater = 0;
		window.SetTitle(std::format("Frame Time: {:^4}ms, FPS: {:.0f}", timeQuery.DeltaTime(), 1e3 / timeQuery.DeltaTime()));
	}

	return 0;
}

void ProcessInput(const Window& window, Camera& camera, const float deltaTime)
{
	if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) window.Close();

	const float moveOffset = 10 * deltaTime;

	if (Input::GetKey(GLFW_KEY_W)) camera.Move(0, 0, moveOffset);
	if (Input::GetKey(GLFW_KEY_S)) camera.Move(0, 0, -moveOffset);
	if (Input::GetKey(GLFW_KEY_A)) camera.Move(-moveOffset, 0, 0);
	if (Input::GetKey(GLFW_KEY_D)) camera.Move(moveOffset, 0, 0);
	if (Input::GetKey(GLFW_KEY_SPACE)) camera.Move(0, moveOffset, 0);
	if (Input::GetKey(GLFW_KEY_LEFT_CONTROL)) camera.Move(0, -moveOffset, 0);

	if (Input::GetKeyDown(GLFW_KEY_LEFT)) camera.Rotate(0, -30);
	if (Input::GetKeyDown(GLFW_KEY_RIGHT)) camera.Rotate(0, 30);
	if (Input::GetKeyDown(GLFW_KEY_UP)) camera.Rotate(30, 0);
	if (Input::GetKeyDown(GLFW_KEY_DOWN)) camera.Rotate(-30, 0);
}
