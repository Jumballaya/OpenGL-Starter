#pragma once

#include <iostream>
#include <cstdint>

#include <glm/glm.hpp>
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


	Window() {
		window = nullptr;
	}

	~Window() {
		destroy();
	}

	GLFWwindow* getWindowPointer() {
		return window;
	};

	void getSize(int* width, int* height) {
		glfwGetFramebufferSize(window, width, height);
	};

	bool shouldClose() {
		return glfwWindowShouldClose(window);
	};

	void update() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void setup(int width, int height) {
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

	void destroy() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

private:
	GLFWwindow* window;
};