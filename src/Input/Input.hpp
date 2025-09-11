#pragma once
#include <map>

#include "GLFW/glfw3.h"

class Input
{
public:
	explicit Input(GLFWwindow* window);
	static void Process();

	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);

	static bool GetButton(int button);
	static bool GetButtonDown(int button);
	static bool GetButtonUp(int button);

private:
	static GLFWwindow* window;

	static std::map<int, bool> lastKeyboard;
	static std::map<int, bool> keyboard;

	static std::map<int, bool> lastMouse;
	static std::map<int, bool> mouse;
};
