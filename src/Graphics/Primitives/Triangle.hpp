#pragma once
#include <array>
#include <initializer_list>
#include <glm/vec3.hpp>

#include "Graphics/Material.hpp"

class Triangle
{
public:
	static constexpr int VERTEX_SIZE = 3;
	static constexpr int VERTEX_COUNT = 3;
	static constexpr int VERTEX_ARRAY_SIZE = VERTEX_SIZE * VERTEX_COUNT;

public:
	Triangle(const std::initializer_list<glm::vec3>& positions, Material& mat);
	~Triangle();

	void Draw() const;

	std::array<glm::vec3, VERTEX_COUNT> GetVertices() const;

private:
	unsigned int VBO, VAO;
	float vertices[VERTEX_ARRAY_SIZE]{};
	Material& material;
};
