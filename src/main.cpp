#include <chrono>
#include <map>
#include <print>
#include <random>

#include "glad/glad.h"
#include "glm/gtc/random.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Primitives/Shape.hpp"

class Camera
{
public:
	glm::vec3 position;
};

enum class BlockType : int
{
	Air,
	Stone,
	Grass
};

struct Block
{
	BlockType type;
};

#define CHUNK_WIDTH 16
#define CHUNK_LENGTH 16
#define CHUNK_HEIGHT 256

std::map<int, bool> lastFrame;

void ProcessInput(const Window& window, int& currentLevel, std::vector<Block>& chunk, const unsigned int& ssbo);

void SetBlock(std::vector<Block>& chunk, const uint8_t x, const uint16_t y, const uint8_t z, const Block block)
{
	chunk[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_LENGTH] = block;
}

Block GetBlock(const std::vector<Block>& chunk, const uint8_t x, const uint16_t y, const uint8_t z)
{
	return chunk[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_LENGTH];
}

int main()
{
	Window window(640, 640);

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

	std::vector<Block> chunk(CHUNK_WIDTH * CHUNK_LENGTH * CHUNK_HEIGHT);

	for (auto i = 0; i < CHUNK_WIDTH * CHUNK_LENGTH * CHUNK_HEIGHT; i++)
	{
		auto x = i % CHUNK_WIDTH;
	}

	for (auto y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (auto z = 0; z < CHUNK_LENGTH; z++)
		{
			for (auto x = 0; x < CHUNK_WIDTH; x++)
			{
				if (y < 60) SetBlock(chunk, x, y, z, Block(BlockType::Stone));
				else if (y < 63) SetBlock(chunk, x, y, z, Block(BlockType::Grass));
				else SetBlock(chunk, x, y, z, Block(BlockType::Air));
			}
		}
	}

#pragma endregion

	unsigned int query;
	glGenQueries(1, &query);
	unsigned int ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<long long>(chunk.size() * sizeof(Block)), &chunk[0],
	             GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	const auto randSeed = rand();

	computeShader->Use();

	computeShader->SetFloat("camFocalLength", 1.0f);
	computeShader->SetInt("texWidth", texWidth);
	computeShader->SetInt("texHeight", texHeight);
	computeShader->SetFloat("randSeed", static_cast<float>(randSeed));
	computeShader->SetInt("chunkWidth", CHUNK_WIDTH);
	computeShader->SetInt("chunkLength", CHUNK_LENGTH);
	computeShader->SetInt("chunkHeight", CHUNK_HEIGHT);

	int currentLevel = 64;

	auto titleUpdater = std::chrono::high_resolution_clock::now();
	while (!window.ShouldClose())
	{
		glBeginQuery(GL_TIME_ELAPSED, query);

		computeShader->Use();
		computeShader->SetVec3("camCenter", camera.position);
		computeShader->SetInt("currentLevel", CHUNK_HEIGHT);
		computeShader->SetInt("currentLevel", currentLevel);

		texture->Clear();
		glDispatchCompute(static_cast<unsigned int>(texWidth) / 32, static_cast<unsigned int>(texHeight) / 32, 1);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		window.Render();

		glEndQuery(GL_TIME_ELAPSED);

		/*const int blockIndex = glGetProgramResourceIndex(computeShader->ID(), GL_SHADER_STORAGE_BLOCK, "Spheres");
		int numActiveVars = 0;
		constexpr GLenum glNumActiveVariables[] = {GL_NUM_ACTIVE_VARIABLES};
		glGetProgramResourceiv(computeShader->ID(), GL_SHADER_STORAGE_BLOCK, blockIndex, 1,
		                       glNumActiveVariables, 1, nullptr, &numActiveVars);

		constexpr GLenum glActiveVariables[] = {GL_ACTIVE_VARIABLES};
		std::vector<int> vars(numActiveVars);
		glGetProgramResourceiv(computeShader->ID(), GL_SHADER_STORAGE_BLOCK, blockIndex, 1,
		                       glActiveVariables, numActiveVars, nullptr, vars.data());

		for (auto var: vars)
		{
			char nameBuf[128];
			GLsizei length = 0;

			glGetProgramResourceName(computeShader->ID(), GL_BUFFER_VARIABLE, var, sizeof(nameBuf), &length, nameBuf);

			int results[4];
			constexpr GLenum props[] =
			{
				GL_OFFSET,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE,
				GL_TOP_LEVEL_ARRAY_STRIDE
			};

			glGetProgramResourceiv(computeShader->ID(),
			                       GL_BUFFER_VARIABLE,
			                       var,
			                       4, props,
			                       4, nullptr,
			                       results);

			std::println("{} offset={} arraystride={} matrixstide={} toplevelarraystride{}", nameBuf, results[0], results[1],
			             results[2], results[3]);
		}*/

		int available = 0;
		while (!available)
			glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &available);
		unsigned long long timeElapsed;
		glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);
		long double deltaTime = static_cast<double>(timeElapsed) / 1e6;
		ProcessInput(window, currentLevel, chunk, ssbo);

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

void UpdateKey(const Window& window, std::map<int, bool>& frame, const int key)
{
	frame[key] = window.GetKey(key) == GLFW_PRESS;
}

bool WasPressedThisFrame(const std::map<int, bool>& lastFrame, const std::map<int, bool>& currentFrame, const int key)
{
	return currentFrame.at(key) && !lastFrame.at(key);
}

void ProcessInput(const Window& window, int& currentLevel, std::vector<Block>& chunk, const unsigned int& ssbo)
{
	if (window.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		window.Close();
	}

	std::map<int, bool> frame;

	UpdateKey(window, frame, GLFW_KEY_W);
	UpdateKey(window, frame, GLFW_KEY_S);
	UpdateKey(window, frame, GLFW_KEY_A);
	UpdateKey(window, frame, GLFW_KEY_D);
	UpdateKey(window, frame, GLFW_KEY_I);
	UpdateKey(window, frame, GLFW_KEY_O);
	UpdateKey(window, frame, GLFW_KEY_P);

	const auto lastLevel = currentLevel;

	if (WasPressedThisFrame(lastFrame, frame, GLFW_KEY_W)) currentLevel++;
	if (WasPressedThisFrame(lastFrame, frame, GLFW_KEY_S)) currentLevel--;
	if (WasPressedThisFrame(lastFrame, frame, GLFW_KEY_A)) currentLevel -= 40;
	if (WasPressedThisFrame(lastFrame, frame, GLFW_KEY_D)) currentLevel += 40;

	auto blockType = -1;

	if (WasPressedThisFrame(lastFrame, frame, GLFW_KEY_I))
	{
		blockType = 0;
	}
	if (WasPressedThisFrame(lastFrame, frame, GLFW_KEY_O))
	{
		blockType = 1;
	}
	if (WasPressedThisFrame(lastFrame, frame, GLFW_KEY_P))
	{
		blockType = 2;
	}

	if (blockType != -1)
	{
		for (auto z = 0; z < CHUNK_LENGTH; z++)
		{
			for (auto x = 0; x < CHUNK_WIDTH; x++)
			{
				chunk[x + z * CHUNK_WIDTH + currentLevel * CHUNK_WIDTH * CHUNK_LENGTH].type = static_cast<BlockType>(
					blockType);
			}
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER,
		                static_cast<long long>(currentLevel * CHUNK_WIDTH * CHUNK_LENGTH * sizeof(Block)),
		                CHUNK_WIDTH * CHUNK_LENGTH * sizeof(Block),
		                chunk.data() + currentLevel * CHUNK_WIDTH * CHUNK_LENGTH);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	if (currentLevel < 0 || currentLevel > 255) currentLevel = lastLevel;

	lastFrame = frame;
}
