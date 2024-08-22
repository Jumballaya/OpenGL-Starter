#include "Window.h"

#include <iostream>
#include <cstdint>

#include <glm/glm.hpp>

Window::Window() {
	window = nullptr;
}

Window::~Window() {
	destroy();
}

GLFWwindow* Window::getWindowPointer() {
	return window;
};

void Window::getSize(int* width, int* height) {
	glfwGetFramebufferSize(window, width, height);
};

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
};

void Window::update() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::setup(int width, int height) {
	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << "Error: " << description << std::endl;
	});

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Simple example", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
}

void Window::destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
}