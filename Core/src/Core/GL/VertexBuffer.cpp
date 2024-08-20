#include <glad/glad.h>

#include "VertexBuffer.h"

#include <iostream>

namespace Core {
	namespace GL {
		VertexBuffer::~VertexBuffer() {
			glDeleteBuffers(1, &vbo);
		}

		VertexBuffer::VertexBuffer(unsigned int size, float* data) {
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		};

		void VertexBuffer::bind() {
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
		};

		void VertexBuffer::unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		};
	}
}