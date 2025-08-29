#include "Shader.hpp"

#include <fstream>
#include <print>
#include <sstream>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "glad/glad.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	const auto vertexCode = ReadCodeFromFile(vertexPath);
	const auto fragmentCode = ReadCodeFromFile(fragmentPath);

	const auto vertex = CreateShader(vertexCode, ShaderType::Vertex);
	const auto fragment = CreateShader(fragmentCode, ShaderType::Fragment);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	CheckErrors(id, ShaderType::Program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::Use() const
{
	glUseProgram(ID());
}

int Shader::GetLocation(const std::string& name) const
{
	return glGetUniformLocation(id, name.c_str());
}


void Shader::SetBool(const std::string& name, const bool value) const
{
	glUniform1i(GetLocation(name), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, const int value) const
{
	glUniform1i(GetLocation(name), value);
}

void Shader::SetFloat(const std::string& name, const float value) const
{
	glUniform1f(GetLocation(name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(GetLocation(name), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, const float x, const float y) const
{
	glUniform2f(GetLocation(name), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(GetLocation(name), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const float x, const float y, const float z) const
{
	glUniform3f(GetLocation(name), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(GetLocation(name), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const float x, const float y, const float z, const float w) const
{
	glUniform4f(GetLocation(name), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value) const
{
	glUniformMatrix2fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) const
{
	glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
}


unsigned int Shader::ID() const
{
	return id;
}

std::string Shader::ReadCodeFromFile(const std::string& filePath)
{
	std::ifstream shaderFile;
	std::string shaderCode;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(filePath);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::println("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", e.what());
	}

	return shaderCode;
}

unsigned int Shader::CreateShader(const std::string& shaderCode, const ShaderType type)
{
	const auto shader = glCreateShader(type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	const auto code = shaderCode.c_str();

	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);
	CheckErrors(shader, type);

	return shader;
}

void Shader::CheckErrors(const unsigned int id, const ShaderType type)
{
	int success;
	char infoLog[512];
	if (type != ShaderType::Program)
	{
		glGetProgramiv(id, GL_COMPILE_STATUS, &success);
		if (success) return;

		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::println("ERROR::SHADER_COMPILATION_ERROR of type {}\n{}",
		             type == ShaderType::Vertex ? "Vertex" : "Fragment", infoLog);;
		return;
	}

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (success) return;

	glGetShaderInfoLog(id, 512, nullptr, infoLog);
	std::println("ERROR::SHADER_LINKING_ERROR of type {}\n{}", "Program", infoLog);
}
