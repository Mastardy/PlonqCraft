#pragma once
#include <string>

#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

class Shader
{
private:
	enum class ShaderType
	{
		Vertex,
		Fragment,
		Program
	};

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Use() const;

	int GetLocation(const std::string& name) const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec2(const std::string& name, float x, float y) const;

	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;

	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w) const;

	void SetMat2(const std::string& name, const glm::mat2& value) const;
	void SetMat3(const std::string& name, const glm::mat3& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;

	unsigned int ID() const;

private:
	static void CheckErrors(unsigned int id, ShaderType type);
	static std::string ReadCodeFromFile(const std::string& filePath);
	static unsigned int CreateShader(const std::string& shaderCode, ShaderType type);

private:
	unsigned int id;
};
