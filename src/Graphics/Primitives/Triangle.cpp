#include "Triangle.hpp"

#include <stdexcept>
#include <glad/glad.h>

Triangle::Triangle(const std::initializer_list<glm::vec3>& positions, Material& mat): material(mat)
{
	if (positions.size() != VERTEX_COUNT) throw std::invalid_argument("Invalid number of positions");

	for (auto i = 0; i < positions.size(); i++)
	{
		vertices[i * VERTEX_COUNT] = positions.begin()[i].x;
		vertices[i * VERTEX_COUNT + 1] = positions.begin()[i].y;
		vertices[i * VERTEX_COUNT + 2] = positions.begin()[i].z;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
}

void Triangle::Draw() const
{
	glUseProgram(material.program);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

std::array<glm::vec3, Triangle::VERTEX_COUNT> Triangle::GetVertices() const
{
	std::array<glm::vec3, VERTEX_COUNT> points{};
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		points[i] = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
	}

	return points;
}

Triangle::~Triangle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
