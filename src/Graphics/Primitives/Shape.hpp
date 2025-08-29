#pragma once
#include <initializer_list>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "Graphics/Material.hpp"

class Shape
{
public:
	Shape(const std::initializer_list<glm::vec3>& _vertices, const std::initializer_list<int>& _indices,
	      const glm::vec3& _position, std::shared_ptr<Material> mat);

	Shape(const Shape&) = delete;
	Shape& operator=(const Shape&) = delete;

	Shape(Shape&& other) noexcept : VBO(other.VBO), VAO(other.VAO), EBO(other.EBO),
	                                vertices(std::move(other.vertices)), indices(std::move(other.indices)),
	                                material(std::move(other.material)), position(other.position)
	{
		other.VAO = other.VBO = other.EBO = 0;
	}

	Shape& operator=(Shape&& other) noexcept
	{
		if (this == &other) return *this;

		vertices = std::move(other.vertices);
		indices = std::move(other.indices);
		position = other.position;
		material = other.material;

		VAO = other.VAO;
		VBO = other.VBO;
		EBO = other.EBO;

		other.VAO = other.VBO = other.EBO = 0;

		return *this;
	}

	~Shape();

	void Draw() const;

private:
	unsigned int VBO{}, VAO{}, EBO{};
	std::vector<float> vertices{};
	std::vector<unsigned int> indices{};
	std::shared_ptr<Material> material;
	glm::vec3 position;
};
