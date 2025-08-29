#pragma once

#include <vector>
#include <GLFW/glfw3.h>

#include "Primitives/Shape.hpp"

class Window
{
public:
	explicit Window(int width = 800, int height = 600);

	~Window();

	bool ShouldClose() const;
	void Close() const;

	void Render() const;

	void AddShape(std::unique_ptr<Shape> shape);

	int GetKey(int key) const;

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* window;
	std::vector<std::unique_ptr<Shape> > shapes;
};
