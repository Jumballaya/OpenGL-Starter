#pragma once

#include <iostream>

#include <glad/glad.h>

namespace Core {
  namespace GL {
    class ElementBuffer {
    public:
        ElementBuffer() = default;
		~ElementBuffer() {
			if (ebo == 0) return;
			glDeleteBuffers(1, &ebo);
		};

		void initialize() { glCreateBuffers(1, &ebo); }

		void load(unsigned int size, unsigned int* data) {
			glNamedBufferStorage(ebo, size, data, GL_DYNAMIC_STORAGE_BIT);
			glBindBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, ebo, 0, size);
			sizeInBytes = size;
		}

		void update(unsigned int size, unsigned int* data) {
			glNamedBufferSubData(ebo, 0, size, data);
			sizeInBytes = size;
		}

		void bind(unsigned int bindPoint) {
			glBindBufferRange(GL_ELEMENT_ARRAY_BUFFER, bindPoint, ebo, 0, sizeInBytes);
		}

		void unbind() {} // API completeness for now

		GLuint getId() { return ebo; }

    private:
        GLuint  ebo = 0;
        unsigned int sizeInBytes = 0;
    };

  }

}