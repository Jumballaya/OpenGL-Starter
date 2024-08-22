#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <Core/Core.h>
#include "Gui.h"


float vertices[] = {
	// X Y Z U V

	// FRONT FACE
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,

	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, 1.0f, 0.0f, 1.0f,

	// BACK FACE
	 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, -1.0f, 1.0f, 1.0f,

	 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	 1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

	 // LEFT FACE
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f, 1.0f, 1.0f,

	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

	// RIGHT FACE
	1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	1.0f,  1.0f, -1.0f, 1.0f, 1.0f,

	1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
	1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

	// TOP FACE
	-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,

	-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

	// BOTTOM FACE
	 1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

	 1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
};

Core::Render::Camera camera;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera.setProjection(glm::radians(70.0f), (float)width / (float)height, 0.01f, 100.0f);
}

int main()
{
	glfwSetErrorCallback([](int error, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
	});

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Simple example", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	std::vector<Core::GL::VertexAttribute> attributes;
	attributes.push_back({ 3, 0 });
	attributes.push_back({ 2, 12 });

	Core::GL::VertexArray vao;
	vao.addBuffer(sizeof(vertices), vertices, 20, attributes);

	Core::GL::Shader shader("shaders/uv/vertex.glsl", "shaders/uv/fragment.glsl");

	Core::Render::Transform model;
	camera.view.translate(glm::vec3(0.0f, 0.0f, -4.0f));

	int w, h, comp;
	const uint8_t* img = stbi_load("textures/stucco.png", &w, &h, &comp, 3);

	GLuint texture;
	glActiveTexture(GL_TEXTURE1);
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(texture, 1, GL_RGB8, w, h);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTextureSubImage2D(texture, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTextures(0, 1, &texture);

	shader.bind();
	shader.uniform_i("u_texture", 1);
	shader.uniform_m("u_model_matrix", 4, &model.matrix()[0][0]);
	shader.uniform_m("u_view_matrix", 4, &camera.viewMatrix()[0][0]);
	shader.uniform_m("u_projection_matrix", 4, &camera.projectionMatrix()[0][0]);
	shader.unbind();

	float rot = 0.0f;

	Gui gui;
	glfwSetWindowUserPointer(window, &gui);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setKeyCallback(window, key, scancode, action, mods);
	});
	glfwSetCursorPosCallback(window, [](auto* window, double x, double y) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setCursorPosCallback(window, x, y);
	});
	glfwSetMouseButtonCallback(window, [](auto* window, int button, int action, int mods) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setMouseButtonCallback(window, button, action, mods);
	});
	glfwSetScrollCallback(window, [](auto* window, double xOffset, double yOffset) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setScrollCallback(window, xOffset, yOffset);
	});
	glfwSetCharCallback(window, [](auto* window, unsigned int c) {
		auto gui = static_cast<Gui*>(glfwGetWindowUserPointer(window));
		gui->setCharCallback(window, c);
	});

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw model
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		vao.bind();
		shader.bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, vao.vertexCount());
		vao.unbind();
		shader.unbind();

		// Draw gui
		gui.Draw(width, height);

		// Update model
		model.rotateX(rot);
		model.rotateZ(rot * 0.333);
		model.rotateX(-rot);
		shader.bind();
		shader.uniform_m("u_model_matrix", 4, &model.matrix()[0][0]);
		shader.uniform_m("u_view_matrix", 4, &camera.viewMatrix()[0][0]);
		shader.uniform_m("u_projection_matrix", 4, &camera.projectionMatrix()[0][0]);
		shader.unbind();
		rot += glm::radians(0.5f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
