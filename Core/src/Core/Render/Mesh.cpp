#include "Mesh.h"

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
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

			glBindVertexArray(0);
		}

		void Mesh::draw(GL::Shader& shader) {
			for (int i = 0; i < textures.size(); i++) {
				textures[i]->bind(i);
				shader.uniform_i(textures[i]->name, i);
			}

			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}