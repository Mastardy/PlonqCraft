#include "Shape.hpp"

#include <print>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "GLFW/glfw3.h"

Shape::Shape(const std::initializer_list<glm::vec3>& _vertices, const std::initializer_list<int>& _indices,
             const glm::vec3& _position, std::shared_ptr<Material> mat): material(std::move(mat)), position(_position)
{
	for (auto i = 0; i < _vertices.size(); i++)
	{
		vertices.push_back(_vertices.begin()[i].x);
		vertices.push_back(_vertices.begin()[i].y);
		vertices.push_back(_vertices.begin()[i].z);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long long>(indices.size() * sizeof(unsigned int)), &indices[0],
	             GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glBindVertexArray(0);
}

void Shape::Draw() const
{
	material->Use();
	glBindVertexArray(VAO);

	const auto shaderPtr = material->GetShader();
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(static_cast<float>(static_cast<int>(glfwGetTime()) % 4 * 90)),
	                    glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.5f));

	const auto view = glm::lookAt(glm::vec3(-7, 4, -7), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	const auto projection = glm::perspective(glm::radians(60.0f), 800.f / 600.f, 0.1f, 100.0f);

	if (const auto shader = shaderPtr.lock())
	{
		shader->SetMat4("model", model);
		shader->SetMat4("view", view);
		shader->SetMat4("projection", projection);
	}

	glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

Shape::~Shape()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
