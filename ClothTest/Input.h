#pragma once

struct GLFWwindow;
enum class KeyboardKey;

class Input
{
public:
	Input(GLFWwindow* a_window);

	bool isKeyPressed(KeyboardKey a_key)const;

private:
	GLFWwindow* m_window;
};