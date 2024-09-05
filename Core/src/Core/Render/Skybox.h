#pragma once
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Core/GL/gl.h"
#include "Core/Render/Bitmap.h"
#include "Core/Render/Camera.h"

namespace Core {
	namespace Render {
		static struct SkyboxFrameData {
		public:
			glm::mat4 model;
			glm::mat4 MVP;
			glm::vec4 cameraPos;
		};
		class Skybox {
		public:
			Skybox() = default;

			void initialize() {
				vao.initialize();
				shader.initialize();
				cubemap.initialize();
				ubo.initialize();
			}

			void load(std::string image, BitmapFormat fmt) {
				ubo.load(0);
				shader.load("shaders/skybox/vertex.glsl", "shaders/skybox/fragment.glsl");
				Bitmap bmp = Bitmap::GenerateCubeMap(image, fmt);
				if (fmt == BitmapFormat_Float) {
					cubemap.load(bmp, GL::TextureOptions::CubeMapf());
				}
				if (fmt == BitmapFormat_UnsignedByte) {
					cubemap.load(bmp, GL::TextureOptions::CubeMap());
				}
			}

			void draw(Camera& camera) {
				glDisable(GL_CULL_FACE);
				vao.bind();
				glm::vec3 camPos = camera.getPosition();
				camera.moveTo(glm::vec3(0.0f, 0.0f, 0.0f));
				glm::mat4 m = glm::mat4(1.0f);
				glm::mat4 v = camera.getViewMatrix();
				glm::mat4 p = camera.getProjectionMatrix();
				ubo.update({ .model = m, .MVP = p * v, .cameraPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) });
				ubo.bind(0);
				cubemap.bind(1);
				shader.bind();
				glDrawArrays(GL_TRIANGLES, 0, 36);
				camera.moveTo(camPos);
			}

		private:
			GL::VertexArray vao;
			GL::Shader shader;
			GL::Texture cubemap;
			GL::UniformBuffer<SkyboxFrameData> ubo;
			glm::mat4 modelMatrix = glm::mat4(1.0f);
		};
	}
}