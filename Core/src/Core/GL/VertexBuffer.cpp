#include <glad/glad.h>

#include "VertexBuffer.h"

#include <iostream>

namespace Core {
	namespace GL {
		void VertexBuffer::bind(unsigned int bindPoint = 0) {
			glBindBufferRange(GL_ARRAY_BUFFER, bindPoint, vbo, 0, size);
		};

		void VertexBuffer::unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		};

		void VertexBuffer::setup() {
			glCreateBuffers(1, &vbo);
		}

		void VertexBuffer::load(unsigned int size, float* data) {
			glNamedBufferStorage(vbo, size, data, GL_DYNAMIC_STORAGE_BIT);
			glBindBufferRange(GL_ARRAY_BUFFER, 0, vbo, 0, size);
			this->size = size;
		}

		void VertexBuffer::update(unsigned int size, float* data) {
			glNamedBufferSubData(vbo, 0, size, data);
		}

		void VertexBuffer::destroy() {
			glDeleteBuffers(1, &vbo);
		}

		unsigned int VertexBuffer::getId() {
			return vbo;
		}
	}
}