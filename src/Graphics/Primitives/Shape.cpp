#include "Shape.hpp"

#include <glad/glad.h>

Shape::Shape(const std::initializer_list<glm::vec3>& positions, const std::initializer_list<int>& _indices,
             Material& mat): material(mat)
{
	for (auto i = 0; i < positions.size(); i++)
	{
		vertices.push_back(positions.begin()[i].x);
		vertices.push_back(positions.begin()[i].y);
		vertices.push_back(positions.begin()[i].z);
	}

	for (auto i = 0; i < _indices.size(); i++)
	{
		this->indices.push_back(_indices.begin()[i]);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(vertices.size() * sizeof(float)), &vertices[0],
	             GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long long>(indices.size() * sizeof(int)), &indices[0],
	             GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
}

void Shape::Draw() const
{
	glUseProgram(material.program);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

Shape::~Shape()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
