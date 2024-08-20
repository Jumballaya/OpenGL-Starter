#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Core.h>


float vertices[] = {
	-1.0, -1.0, 0.0, 0.0, 0.0,
	 1.0, -1.0, 0.0, 1.0, 0.0,
	 1.0,  1.0, 0.0, 1.0, 1.0,
	-1.0, -1.0, 0.0, 0.0, 0.0,
	 1.0,  1.0, 0.0, 1.0, 1.0,
	-1.0,  1.0, 0.0, 0.0, 1.0
};


// Camera camera;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	// camera.setProjection(glm::radians(70.0f), (float)width / (float)height, 0.01f, 100.0f);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Spinning Cube", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::vector<Core::GL::VertexAttribute> attributes;
	attributes.push_back({ 3, 0 });
	attributes.push_back({ 2, 12 });

	Core::GL::VertexArray vao;
	vao.addBuffer(sizeof(vertices), vertices, 20, attributes);

	Core::GL::Shader shader("shaders/uv/vertex.glsl", "shaders/uv/fragment.glsl");

	// Transform model;
	// camera.view.translate(glm::vec3(0.0f, 0.0f, -4.0f));

	// shader.bind();
	// shader.uniform_m("u_model_matrix", 4, &model.matrix()[0][0]);
	// shader.uniform_m("u_view_matrix", 4, &camera.viewMatrix()[0][0]);
	// shader.uniform_m("u_projection_matrix", 4, &camera.projectionMatrix()[0][0]);
	// shader.unbind();

	float rot = 0.0f;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw triangle
		vao.bind();
		shader.bind();

		glDrawArrays(GL_TRIANGLES, 0, vao.vertexCount());

		glfwSwapBuffers(window);
		glfwPollEvents();

		// model.rotateX(rot);
		// model.rotateZ(rot * 0.333);
		// model.rotateX(-rot);
		// shader.bind();
		// shader.uniform_m("u_model_matrix", 4, &model.matrix()[0][0]);
		// shader.uniform_m("u_view_matrix", 4, &camera.viewMatrix()[0][0]);
		// shader.uniform_m("u_projection_matrix", 4, &camera.projectionMatrix()[0][0]);
		// shader.unbind();
		rot += glm::radians(0.5f);
	}

	glfwTerminate();

	return 0;
}