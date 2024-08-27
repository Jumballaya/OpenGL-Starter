#include <stb/stb_image.h>

#include "App.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <queue>
#include <vector>


int main() {
	App app;

	Core::GL::Shader shader("shaders/model/vertex.glsl", "shaders/model/fragment.glsl");
	Core::Render::Model duckModel("models/duck/Duck.gltf");
	Entity ent(&duckModel, &shader);

	ent.transform.scale(glm::vec3(0.05f));
	ent.transform.translate(glm::vec3(0.0f, -4.0f, -8.0f));

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	app.run([&shader, &ent](App* app, int width, int height) {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		shader.bind();
		app->camera.update();
		ent.draw();
		shader.unbind();
	});

	app.destroy();

	exit(EXIT_SUCCESS);
}
