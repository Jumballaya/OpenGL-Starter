#include "Entity.h"

void Entity::draw() {
	if (model != nullptr && shader != nullptr) {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		shader->bind();
		shader->uniform_m("u_model_matrix", 4, &transform.matrix()[0][0]);
		model->draw(*shader);
		shader->unbind();
	}
}