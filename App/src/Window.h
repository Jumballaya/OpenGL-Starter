#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();
	~Window();

	void setup(int width, int height);

	GLFWwindow* getWindowPointer();
	void getSize(int* width, int* height);
	bool shouldClose();
	void update();

	void destroy();

private:
	GLFWwindow* window;
};