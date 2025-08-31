#pragma once

class Texture
{
public:
	Texture(int width, int height);
	~Texture();

	void Bind() const;
	void SetSize(int width, int height);
	void Clear() const;

private:
	unsigned int texture{};
	unsigned int width{}, height{};
};
