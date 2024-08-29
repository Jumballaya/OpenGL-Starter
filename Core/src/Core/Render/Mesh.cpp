#include "Mesh.h"

#include <vector>
#include <iostream>

#include <glad/glad.h>

namespace Core {
	namespace Render {		
		Mesh::Mesh(
			std::vector<Vertex> v,
			std::vector<GLuint> i,
			std::vector<Core::GL::Texture> t
		) : vertices(v), indices(i), textures(t) {
			setupMesh();
		}

		void Mesh::setup() {
			vao.setup();
		};

		void Mesh::load(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Core::GL::Texture> textures) {
			vao.bind();
			vao.loadElementBuffer(indices.size() * sizeof(GLuint), &indices[0]);
			auto attrList = std::vector<GL::VertexAttribute>({
					{ 3, offsetof(Vertex, position), GL_FLOAT, false },
					{ 3, offsetof(Vertex, normal), GL_FLOAT, false },
					{ 2, offsetof(Vertex, uv), GL_FLOAT, false }
				});
			vao.loadVertexBuffer(vertices.size() * sizeof(Vertex), 32, (float*)&vertices[0], attrList);
			vao.unbind();
		};

		void Mesh::destroy() {
			vao.destroy();
			for (GL::Texture tex : textures) {
				tex.destroy();
			}
			vertices.clear();
			indices.clear();
			textures.clear();
		};

		void Mesh::setupMesh() {
			vao.setup();
			vao.bind();
			vao.loadElementBuffer(indices.size() * sizeof(GLuint), &indices[0]);
			auto attrList = std::vector<GL::VertexAttribute>({
					{ 3, offsetof(Vertex, position), GL_FLOAT, false },
					{ 3, offsetof(Vertex, normal), GL_FLOAT, false },
					{ 2, offsetof(Vertex, uv), GL_FLOAT, false }
				});
			vao.loadVertexBuffer(vertices.size() * sizeof(Vertex), 32, (float*) &vertices[0], attrList);
			vao.unbind();
		}

		void Mesh::draw(GL::Shader& shader) {
			for (int i = 0; i < textures.size(); i++) {
				textures[i].bind(i);
			}

			vao.bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			vao.unbind();
		}
	}
}