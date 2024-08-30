#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// @TODO: window state that collects info from
// GLFW event callbacks
struct WindowState {};

// @TODO: setup window to record the state from the GLFW
// events into the WindowState struct
// Create setters/getters on the window class to be able
// to access this data (get mouse position, button clicked, key pressed, etc.)
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