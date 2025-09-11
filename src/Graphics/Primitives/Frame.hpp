#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Graphics/Material.hpp"

struct VertexData
{
	glm::vec3 position;
	glm::vec2 uv;
};

class Frame
{
public:
	explicit Frame(std::shared_ptr<Material> mat);

	Frame(const Frame&) = delete;
	Frame(Frame&& other) = delete;
	Frame& operator=(const Frame&) = delete;
	Frame& operator=(Frame&& other) = delete;

	~Frame();

	void Draw() const;

private:
	unsigned int VBO{}, VAO{};
	std::vector<float> vertices{};
	std::shared_ptr<Material> material;
};
