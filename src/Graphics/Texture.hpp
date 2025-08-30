#pragma once

class Texture
{
public:
	Texture(int width, int height);
	~Texture();

	void Bind() const;
	void SetSize(int width, int height);

private:
	unsigned int texture{};
	unsigned int width{}, height{};
};
