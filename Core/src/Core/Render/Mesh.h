#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Core/GL/gl.h"

namespace Core {
	namespace Render {
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};

		class Mesh {
		public:
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<Core::GL::Texture*> textures;

			Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Core::GL::Texture*> textures);
			~Mesh() = default;

			void draw(GL::Shader& shader);

		private:
			// VertexArray vao;
			// VertexBuffer vbo;
			// ElementBuffer ebo;
			GLuint vao;
			GLuint vbo;
			GLuint ebo;

			void setupMesh();
		};
	}
}

