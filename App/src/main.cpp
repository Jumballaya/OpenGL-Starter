#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "App.h"


int main() {
	App app;

	Core::GL::Shader shader("shaders/uv/vertex.glsl", "shaders/uv/fragment.glsl");
	Core::Render::Model duckModel("models/duck/Duck.gltf");
	Entity ent(&duckModel, &shader);

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	app.run([&shader, &ent](App* app, int width, int height) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Update MVP
		shader.bind();
		shader.uniform_m("u_view_matrix", 4, &app->camera.viewMatrix()[0][0]);
		shader.uniform_m("u_projection_matrix", 4, &app->camera.projectionMatrix()[0][0]);
		shader.unbind();

		// Draw model
		ent.draw();

	});

	app.destroy();

	exit(EXIT_SUCCESS);
}
