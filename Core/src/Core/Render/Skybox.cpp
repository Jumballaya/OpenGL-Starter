#include "Skybox.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/ext.hpp>

namespace Core {
	namespace Render {
		void Skybox::setup() {
			vao.setup();
			shader.setup();
			cubemap.setup();
			ubo.setup();
		}

		void Skybox::destroy() {
			ubo.destroy();
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
			glm::vec3 camPos = camera.getPosition();
			camera.moveTo(glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 m = glm::mat4(1.0f);
			glm::mat4 v = camera.getViewMatrix();
			glm::mat4 p = camera.getProjectionMatrix();
			ubo.setData({ .model = m, .MVP = p * v, .cameraPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) });
			ubo.bind(0);
			cubemap.bind(1);
			shader.bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
			camera.moveTo(camPos);
		}
	}
}