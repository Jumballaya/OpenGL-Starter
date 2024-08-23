#include "Entity.h"

void Entity::draw() {
	if (model != nullptr && shader != nullptr) {
		shader->bind();
		shader->uniform_m("u_model_matrix", 4, &transform.matrix()[0][0]);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		model->draw(*shader);
		shader->unbind();
	}
}