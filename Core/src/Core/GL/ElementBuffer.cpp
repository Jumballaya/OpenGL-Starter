#include "ElementBuffer.h"
#include <iostream>
#include <glad/glad.h>

namespace Core {
	namespace GL {

		void ElementBuffer::bind(unsigned int bindPoint) {
			glBindBufferRange(GL_ELEMENT_ARRAY_BUFFER, bindPoint, ebo, 0, size);
		}

		void ElementBuffer::setup() {
			glCreateBuffers(1, &ebo);
		}

		void ElementBuffer::load(unsigned int size, unsigned int* data) {
			glNamedBufferStorage(ebo, size, data, GL_DYNAMIC_STORAGE_BIT);
			glBindBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, ebo, 0, size);
			this->size = size;
		}

		void ElementBuffer::update(unsigned int size, unsigned int* data) {
			glNamedBufferSubData(ebo, 0, size, data);
		}

		void ElementBuffer::destroy() {
			glDeleteBuffers(1, &ebo);
		}

		unsigned int ElementBuffer::getId() {
			return ebo;
		}
	
	}
}