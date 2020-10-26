#include "Input.h"
#include <GLFW/glfw3.h>
#include "KeyboardKey.h"

Input::Input(GLFWwindow* a_window)
	: m_window(a_window)
{}

bool Input::isKeyPressed(KeyboardKey a_key)const
{
	return glfwGetKey(m_window, static_cast<int>(a_key)) == GLFW_PRESS;
}