#pragma once

#include <GLFW/glfw3.h>

#include "Primitives/Frame.hpp"

class Window
{
public:
	explicit Window(int width = 800, int height = 600);

	~Window();

	bool ShouldClose() const;
	void Close() const;

	void Render() const;

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	std::shared_ptr<Texture> GetTexture() const { return texture; }

	int GetKey(int key) const;

	GLFWwindow* GetGLFWWindow() const { return window; }

	void SetTitle(const std::string& title) const;

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	int width, height;
	GLFWwindow* window;
	std::unique_ptr<Frame> frame;
	std::shared_ptr<Texture> texture;
};
