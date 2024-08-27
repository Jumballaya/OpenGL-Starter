#include "Mesh.h"

#include <vector>
#include <iostream>

namespace Core {
	namespace Render {		
		Mesh::Mesh(
			std::vector<Vertex> v,
			std::vector<GLuint> i,
			std::vector<Core::GL::Texture*> t
		) : vertices(v), indices(i), textures(t) {
			setupMesh();
		}

		void Mesh::setupMesh() {
			vao.setup();
			vao.bind();
			vao.loadElementBuffer(indices.size() * sizeof(GLuint), &indices[0]);
			auto attrList = std::vector<GL::VertexAttribute>({
					{ 3, offsetof(Vertex, position) },
					{ 3, offsetof(Vertex, normal) },
					{ 2, offsetof(Vertex, uv) }
				});
			vao.loadVertexBuffer(vertices.size() * sizeof(Vertex), 32, (float*) &vertices[0], attrList);
			vao.unbind();
		}

		void Mesh::draw(GL::Shader& shader) {
			for (int i = 0; i < textures.size(); i++) {
				if (textures[i] != nullptr) {
					textures[i]->bind(i);
					shader.uniform_i(textures[i]->name, i);
				}
			}

			vao.bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			vao.unbind();
		}
	}
}