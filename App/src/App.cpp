#include "App.h"
#include <imgui/imgui.h>

App::App() {
	setup();
}

App::~App() {
	destroy();
}

void App::setup() {
	window.setup(800, 600);
	camera.setup();
	gui.setup();

	glfwSetWindowUserPointer(window.getWindowPointer(), &gui);
	glfwSetWindowUserPointer(window.getWindowPointer(), &camera);
	glfwSetKeyCallback(window.getWindowPointer(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setKeyCallback(window, key, scancode, action, mods);
	});
	glfwSetCursorPosCallback(window.getWindowPointer(), [](auto* window, double x, double y) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setCursorPosCallback(window, x, y);
	});
	glfwSetMouseButtonCallback(window.getWindowPointer(), [](auto* window, int button, int action, int mods) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setMouseButtonCallback(window, button, action, mods);
	});
	glfwSetScrollCallback(window.getWindowPointer(), [](auto* window, double xOffset, double yOffset) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setScrollCallback(window, xOffset, yOffset);
	});
	glfwSetCharCallback(window.getWindowPointer(), [](auto* window, unsigned int c) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setCharCallback(window, c);
	});
	glfwSetFramebufferSizeCallback(window.getWindowPointer(), [](auto* window, int width, int height) {
		glViewport(0, 0, width, height);
		auto camera = static_cast<Core::Render::Camera*>(glfwGetWindowUserPointer(window));
		camera->setProjection(glm::radians(70.0f), (float)width / (float)height, 0.01f, 100.0f);
	});
}

void App::destroy() {
	window.destroy();
	gui.destroy();
}

void App::run(const std::function <void(App*, int, int)>& fn) {
	while (!window.shouldClose()) {
		int width, height;
		window.getSize(&width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		gui.startFrame(width, height);
		fn(this, width, height);

		gui.draw(width, height);
		window.update();
	}
}