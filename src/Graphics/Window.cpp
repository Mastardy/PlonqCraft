#include <glad/glad.h>

#include "Window.hpp"

#include <print>

Window::Window(const int width, const int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Hello Worlders!", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::Close() const
{
	glfwSetWindowShouldClose(window, true);
}

void Window::Render() const
{
	glClearColor(0.1f, 0.1f, 0.125f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& t: triangles)
	{
		t.Draw();
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::AddTriangle(const Triangle& triangle)
{
	triangles.push_back(triangle);
}


int Window::GetKey(const int key) const
{
	return glfwGetKey(window, key);
}


void Window::FramebufferSizeCallback(GLFWwindow*, const int width, const int height)
{
	glViewport(0, 0, width, height);
}
