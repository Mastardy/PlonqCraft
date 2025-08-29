#include "Material.hpp"

Material::Material(const std::shared_ptr<Shader>& shader): shader(shader) {}

void Material::Use() const
{
	shader->Use();
}

unsigned int Material::GetProgram() const
{
	return shader->ID();
}

std::weak_ptr<Shader> Material::GetShader() const
{
	return shader;
}
