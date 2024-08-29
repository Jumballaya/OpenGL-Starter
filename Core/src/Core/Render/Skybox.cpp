#include "Skybox.h"

namespace Core {
	namespace Render {
		void Skybox::setup() {
			vao.setup();
			shader.setup();
			cubemap.setup();
		}

		void Skybox::destroy() {
			vao.destroy();
			shader.destroy();
			cubemap.destroy();
		}

		void Skybox::load(std::string image, BitmapFormat fmt) {
			shader.load("shaders/skybox/vertex.glsl", "shaders/skybox/fragment.glsl");
			Bitmap bmp = Bitmap::GenerateCubeMap(image, fmt);
			if (fmt == BitmapFormat_Float) {
				cubemap.load(bmp, GL::TextureOptions::CubeMapf());
			}
			if (fmt == BitmapFormat_UnsignedByte) {
				cubemap.load(bmp, GL::TextureOptions::CubeMap());
			}
		}

		void Skybox::draw(Camera& camera) {
			glDisable(GL_CULL_FACE);
			vao.bind();
			glm::vec3 pos = camera.getPosition();
			camera.moveTo(glm::vec3(0.0f));
			camera.update();
			cubemap.bind(1);
			shader.bind();
			shader.uniform_m("u_model_matrix", 4, &modelMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 32);
			camera.moveTo(pos);
		}
	}
}