#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>


#include "Core/GL/gl.h"

#include <memory>

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
			std::vector<Core::GL::Texture> textures;

			Mesh() = default;
			~Mesh() = default;
			
			// @TODO: Figure out how to get rid of this, Model class needs it now for the return statement of the processMesh method
			Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Core::GL::Texture> textures);
			
			void draw(GL::Shader& shader);

			void setup();
			void load(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Core::GL::Texture> textures);
			void destroy();

		private:
			GL::VertexArray vao;
			void setupMesh();
		};
	}
}

