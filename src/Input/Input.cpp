#include "Input.hpp"

GLFWwindow* Input::window = nullptr;
std::map<int, bool> Input::lastKeyboard = {};
std::map<int, bool> Input::keyboard = {};
std::map<int, bool> Input::lastMouse = {};
std::map<int, bool> Input::mouse = {};

Input::Input(GLFWwindow* window)
{
	Input::window = window;
}

void Input::Process()
{
	lastKeyboard = keyboard;
	lastMouse = mouse;

	for (auto i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_8; i++)
	{
		mouse[i] = glfwGetMouseButton(window, i);
	}

	for (auto i = GLFW_KEY_SPACE; i <= GLFW_KEY_MENU; i++)
	{
		keyboard[i] = glfwGetKey(window, i);
	}
}

bool Input::GetKey(const int key)
{
	return keyboard[key];
}

bool Input::GetKeyDown(const int key)
{
	return keyboard[key] && !lastKeyboard[key];
}

bool Input::GetKeyUp(const int key)
{
	return !keyboard[key] && lastKeyboard[key];
}

bool Input::GetButton(const int button)
{
	return mouse[button];
}

bool Input::GetButtonDown(const int button)
{
	return mouse[button] && !lastMouse[button];
}

bool Input::GetButtonUp(const int button)
{
	return !mouse[button] && lastMouse[button];
}
