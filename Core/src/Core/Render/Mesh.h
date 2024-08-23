#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Core/GL/Shader.h"

namespace Core {
	namespace Render {
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};

		struct Texture {
			GLuint id;
			std::string type;
			std::string path;
		};

		class Mesh {
		public:
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<Texture> textures;

			Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
			~Mesh() = default;

			void Draw(GL::Shader & shader);

		private:
			GLuint vao;
			GLuint vbo;
			GLuint ebo;

			void setupMesh();
		};
	}
}

