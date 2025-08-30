#pragma once
#include <initializer_list>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "Graphics/Material.hpp"

struct VertexData
{
	glm::vec3 position;
	glm::vec2 uv;
};

class Shape
{
public:
	Shape(const std::initializer_list<VertexData>& _vertices, std::shared_ptr<Material> mat);

	Shape(const Shape&) = delete;
	Shape& operator=(const Shape&) = delete;

	Shape(Shape&& other) noexcept : VBO(other.VBO), VAO(other.VAO), vertices(std::move(other.vertices)),
	                                material(std::move(other.material))
	{
		other.VAO = other.VBO = 0;
	}

	Shape& operator=(Shape&& other) noexcept
	{
		if (this == &other) return *this;

		vertices = std::move(other.vertices);
		material = other.material;

		VAO = other.VAO;
		VBO = other.VBO;

		other.VAO = other.VBO = 0;

		return *this;
	}

	~Shape();

	void Draw() const;

private:
	unsigned int VBO{}, VAO{};
	std::vector<float> vertices{};
	std::shared_ptr<Material> material;
};
