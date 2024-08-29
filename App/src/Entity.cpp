#include "Entity.h"

void Entity::setup() {
	shader.setup();
	model.setup();
}

void Entity::loadShader(std::string vertex, std::string fragment) {
	shader.load(vertex, fragment);
}

void Entity::loadModel(std::string path) {
	model.load(path);
}

void Entity::destroy() {
	shader.destroy();
	model.destroy();
}

void Entity::draw() {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	shader.bind();
	shader.uniform_m("u_model_matrix", 4, &transform.matrix()[0][0]);
	model.draw(shader);
	shader.unbind();
}