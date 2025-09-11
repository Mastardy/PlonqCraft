#pragma once
#include <memory>

#include "Shader.hpp"
#include "Texture.hpp"

class Material
{
public:
	explicit Material(const std::shared_ptr<Shader>& shader);
	explicit Material(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture);

	void Use() const;

	unsigned int GetProgram() const;
	std::weak_ptr<Shader> GetShader() const;

private:
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Shader> shader;
};
