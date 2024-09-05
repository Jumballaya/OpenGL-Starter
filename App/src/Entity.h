#pragma once

#include <string>

#include <Core/Core.h>

class Entity {
public:
	Core::Render::Transform transform;

	Entity() = default;
	~Entity() = default;

	void setup() {
		shader.initialize();
		model.setup();
	}

	void loadShader(std::string vertex, std::string fragment) {
		shader.load(vertex, fragment);
	}

	void loadModel(std::string path) {
		model.load(path);
	}

	void draw() {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		shader.bind();
		shader.uniform_m("u_model_matrix", 4, &transform.matrix()[0][0]);
		model.draw(shader);
		shader.unbind();
	}

	Core::Render::Model model;
	Core::GL::Shader shader;
private:
};