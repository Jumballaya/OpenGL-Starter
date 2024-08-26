#include <stb/stb_image.h>

#include "App.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <queue>


int main() {
	App app;
	std::vector<Core::GL::Texture*> textures;

	Core::GL::Shader shader("shaders/model/vertex.glsl", "shaders/model/fragment.glsl");
	Core::Render::Model duckModel("models/duck/Duck.gltf");
	Entity ent(&duckModel, &shader);

	ent.transform.scale(glm::vec3(0.05f));
	ent.transform.translate(glm::vec3(0.0f, -5.0f, -10.0f));

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	while (!app.window.shouldClose()) {
		int width, height;
		app.window.getSize(&width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Update MVP
		shader.bind();
		shader.uniform_m("u_view_matrix", 4, &app.camera.viewMatrix()[0][0]);
		shader.uniform_m("u_projection_matrix", 4, &app.camera.projectionMatrix()[0][0]);
		shader.unbind();

		// Draw model
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		shader.bind();
		ent.draw();
		shader.unbind();


		// gui.draw(width, height);
		app.window.update();
	}

	app.destroy();

	exit(EXIT_SUCCESS);
}
