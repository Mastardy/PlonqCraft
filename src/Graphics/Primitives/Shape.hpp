#pragma once
#include <initializer_list>
#include <vector>
#include <glm/vec3.hpp>

#include "Graphics/Material.hpp"

class Shape
{
public:
	Shape(const std::initializer_list<glm::vec3>& positions, const std::initializer_list<int>& _indices,
	      Material& mat);

	~Shape();

	void Draw() const;

private:
	unsigned int VBO{}, VAO{}, EBO{};
	std::vector<float> vertices{};
	std::vector<int> indices{};
	Material& material;
};
