#include "Material.hpp"

Material::Material(const std::shared_ptr<Shader>& shader): shader(shader) {}

Material::Material(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture): texture(texture),
	shader(shader) {}

void Material::Use() const
{
	// if (texture != nullptr) texture->Bind();
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
