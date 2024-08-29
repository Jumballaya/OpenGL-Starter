#include "App.h"

#include <glm/glm.hpp>

int main() {
	App app;
	Entity ent;
	ent.setup();
	ent.loadShader("shaders/model/vertex.glsl", "shaders/model/fragment.glsl");
	ent.loadModel("models/duck/Duck.gltf");

	ent.transform.scale(glm::vec3(0.05f));
	ent.transform.translate(glm::vec3(0.0f, -4.5f, -10.0f));

	Core::Render::Skybox skybox;
	skybox.setup();
	skybox.load("textures/immenstadter_horn_2k.hdr", Core::Render::BitmapFormat_Float);

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	float spin = 0.0f;
	app.run([&ent, &spin, &skybox](App* app, int width, int height) {
		skybox.draw(app->camera);
		app->camera.update();
		ent.transform.rotateY(spin);
		ent.draw();
		spin += 3.1415f / 180.0f;
		app->gui.drawTextureWindowGL("Duck Texture", ent.model.meshes[0].textures[0].getId());
	});

	ent.destroy();
	skybox.destroy();
	app.destroy();

	exit(EXIT_SUCCESS);
}
