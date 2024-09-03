//#include "App.h"
//
//#include <glm/glm.hpp>
//
//#include <Core/IO/FPSCounter.h>

#include <iostream>
#include <vector>
#include <memory>

#include <glad/glad.h>

class VertexArray {
public:
	VertexArray() = default;
	~VertexArray() { if (vao > 0) glDeleteVertexArrays(1, &vao); }

	// Move
	VertexArray(VertexArray&& other) noexcept {
		vao = other.vao;
		other.vao = 0;
	}

	// Move
	VertexArray& operator=(VertexArray&& other) noexcept {
		vao = other.vao;
		other.vao = 0;
		return *this;
	}

	void initialize() { glCreateVertexArrays(1, &vao); }
	void bind() { glBindVertexArray(vao); }
	void unbind() { glBindVertexArray(0); }

private:
	GLuint vao;
};

int main() {
	//App app;
	//Entity ent;
	//ent.setup();
	//ent.loadShader("shaders/model/vertex.glsl", "shaders/model/fragment.glsl");
	//ent.loadModel("models/duck/Duck.gltf");

	//ent.transform.scale(glm::vec3(0.05f));
	//ent.transform.translate(glm::vec3(0.0f, -4.5f, -10.0f));

	//glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	//float spin = 0.0f;
	//Core::IO::FPSCounter fpsCounter;
	//app.run([&ent, &spin, &fpsCounter](App* app, double dT, int width, int height) {
	//	fpsCounter.tick(dT);
	//	ent.transform.rotateY(spin);
	//	ent.draw();
	//	spin += 3.1415f / 180.0f;
	//	app->gui.drawTextureWindowGL("Duck Texture", ent.model.meshes[0].textures[0].getId());
	//	app->gui.drawFPSDisplayGL(fpsCounter.getFPS());
	//});

	//ent.destroy();
	//app.destroy();
	//exit(EXIT_SUCCESS);

	return 0;
}
