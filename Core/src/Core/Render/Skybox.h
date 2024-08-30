#pragma once
#include <string>

#include <glm/glm.hpp>

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
			~Skybox() = default;

			void setup();
			void load(std::string path, BitmapFormat fmt);
			void destroy();

			void draw(Camera& camera);

		private:
			GL::VertexArray vao;
			GL::Shader shader;
			GL::Texture cubemap;
			GL::UniformBuffer<SkyboxFrameData> ubo;
			glm::mat4 modelMatrix = glm::mat4(1.0f);
		};
	}
}