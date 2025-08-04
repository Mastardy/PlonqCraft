#pragma once
#include <initializer_list>
#include <glm/vec3.hpp>

class Triangle
{
private:
	static constexpr int VERTEX_SIZE = 3;
	static constexpr int VERTEX_COUNT = VERTEX_SIZE * 3;

public:
	Triangle(std::initializer_list<glm::vec3> positions);

private:
	float vertices[VERTEX_COUNT];
};
