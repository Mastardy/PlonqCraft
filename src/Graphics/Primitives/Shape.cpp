#include "Shape.hpp"

#include <print>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "GLFW/glfw3.h"

Shape::Shape(const std::initializer_list<VertexData>& _vertices,
             std::shared_ptr<Material> mat): material(std::move(mat))
{
	for (auto i = 0; i < _vertices.size(); i++)
	{
		vertices.push_back(_vertices.begin()[i].position.x);
		vertices.push_back(_vertices.begin()[i].position.y);
		vertices.push_back(_vertices.begin()[i].position.z);
		vertices.push_back(_vertices.begin()[i].uv.x);
		vertices.push_back(_vertices.begin()[i].uv.y);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(vertices.size() * sizeof(float)), &vertices[0],
	             GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

	glBindVertexArray(0);
}

void Shape::Draw() const
{
	material->Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Shape::~Shape()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
