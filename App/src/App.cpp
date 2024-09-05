#include "App.h"
#include <imgui/imgui.h>

App::App() {
	setup();
}

App::~App() {
	destroy();
}

// @TODO: Move the callback calls to the Window class
// need to find a solution for setting up the gui callback calls
// maybe there is a way to set multiple callbacks with GLFW
void App::setup() {
	window.setup(800, 600);
	auto controller = Core::Render::CameraControllerFPS();
	camera.initialize();
	gui.setup();
	inputs.setup();
	inputs.load(GLFW_KEY_MAP);
	skybox.initialize();
	skybox.load("textures/immenstadter_horn_2k.hdr", Core::Render::BitmapFormat_Float);

	glfwSetWindowUserPointer(window.getWindowPointer(), &gui);
	glfwSetWindowUserPointer(window.getWindowPointer(), &inputs);
	glfwSetKeyCallback(window.getWindowPointer(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setKeyCallback(window, key, scancode, action, mods);

		
		auto inputs = static_cast<Core::IO::Inputs*>(glfwGetWindowUserPointer(window));
		const bool pressed = action != GLFW_RELEASE;
		inputs->updateKeys(key, pressed);
	});
	glfwSetCursorPosCallback(window.getWindowPointer(), [](auto* window, double x, double y) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setCursorPosCallback(window, x, y);

		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		auto inputs = static_cast<Core::IO::Inputs*>(glfwGetWindowUserPointer(window));
		glm::vec2 mousePos = glm::vec2(static_cast<float>(x / w),static_cast<float>(y / h));
		inputs->updateMousePosition(mousePos);
	});
	glfwSetMouseButtonCallback(window.getWindowPointer(), [](auto* window, int button, int action, int mods) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setMouseButtonCallback(window, button, action, mods);

		auto inputs = static_cast<Core::IO::Inputs*>(glfwGetWindowUserPointer(window));
		bool pressed = action == GLFW_PRESS;
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			inputs->updateMouseButton(Core::IO::MouseButton_Left, pressed);
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			inputs->updateMouseButton(Core::IO::MouseButton_Right, pressed);
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			inputs->updateMouseButton(Core::IO::MouseButton_Middle, pressed);
		}
	});
	glfwSetScrollCallback(window.getWindowPointer(), [](auto* window, double xOffset, double yOffset) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setScrollCallback(window, xOffset, yOffset);
	});
	glfwSetCharCallback(window.getWindowPointer(), [](auto* window, unsigned int c) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setCharCallback(window, c);
	});
}

void App::destroy() {
	gui.destroy();
	window.destroy();
}

void App::run(const std::function <void(App*, double, int, int)>& fn) {
	double timeStamp = glfwGetTime();
	float deltaSeconds = 0.0f;
	int width, height;
	window.getSize(&width, &height);
	glViewport(0, 0, width, height);
	camera.setProjection(glm::radians(45.0f), (float)width / (float)height, 0.01f, 100.0f);
	camera.updateData();

	while (!window.shouldClose()) {
		// Delta time
		const double newTimeStamp = glfwGetTime();
		deltaSeconds = static_cast<float>(newTimeStamp - timeStamp);
		timeStamp = newTimeStamp;

		// Screen size
		int frameW, frameH;
		window.getSize(&frameW, &frameH);
		if (width != frameW || height != frameH) {
			width = frameW;
			height = frameH;
			glViewport(0, 0, width, height);
			camera.setProjection(glm::radians(45.0f), (float)width / (float)height, 0.01f, 100.0f);
			camera.updateData();
		}

		// Draw screen
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		gui.startFrame(width, height);
		skybox.draw(camera);
		camera.update(deltaSeconds, inputs);
		camera.ubo.bind(0);
		fn(this, deltaSeconds, width, height);
		gui.draw(width, height);
		window.update();
	}
}