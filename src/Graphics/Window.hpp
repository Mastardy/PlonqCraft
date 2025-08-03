#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
	explicit Window(int width = 800, int height = 600);
	~Window();

	bool ShouldClose() const;
	void ShouldClose(bool) const;

	void Render() const;

	int GetKey(int key) const;

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* window;
};
