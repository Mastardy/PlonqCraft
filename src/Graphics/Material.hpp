#pragma once
#include <memory>

#include "Shader.hpp"

class Material
{
public:
	explicit Material(const std::shared_ptr<Shader>& shader);

	void Use() const;

	unsigned int GetProgram() const;
	std::weak_ptr<Shader> GetShader() const;

private:
	std::shared_ptr<Shader> shader;
};
