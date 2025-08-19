#include "Material.hpp"

#include <glad/glad.h>

Material::Material()
{
	auto vertexShaderSource = "#version 460 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{"
			"gl_Position = vec4(aPos, 1.0);\n"
			"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	auto fragmentShaderSource = "#version 460 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{"
			"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
			"}";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Material::~Material()
{
	glDeleteProgram(program);
}
