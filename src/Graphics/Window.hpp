#pragma once

#include <vector>
#include <GLFW/glfw3.h>

#include "Primitives/Triangle.hpp"

class Window
{
public:
	explicit Window(int width = 800, int height = 600);

	~Window();

	bool ShouldClose() const;

	void Close() const;

	void Render() const;

	int GetKey(int key) const;

	void AddTriangle(const Triangle& triangle);

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* window;
	std::vector<Triangle> triangles;
};
