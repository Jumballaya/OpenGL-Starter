#include "App.h"

int main() {
	App app;
	Entity ent;
	ent.setup();
	ent.loadShader("shaders/model/vertex.glsl", "shaders/model/fragment.glsl");
	ent.loadModel("models/duck/Duck.gltf");

	ent.transform.scale(glm::vec3(0.05f));
	ent.transform.translate(glm::vec3(0.0f, -4.0f, -12.0f));

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	float spin = 0.0f;
	app.run([&ent, &spin](App* app, int width, int height) {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		ent.transform.rotateY(spin);
		ent.draw(app->camera);
		spin += 3.1415f / 180.0f;
	});

	ent.destroy();
	app.destroy();

	exit(EXIT_SUCCESS);
}
