#pragma once

#include <glad/glad.h>

namespace Core {
	namespace GL {
		class VertexBuffer {
		public:
			VertexBuffer() = default;
			~VertexBuffer() {
				if (vbo == 0) return;
				glDeleteBuffers(1, &vbo);
			};

			// Move
			VertexBuffer(VertexBuffer&& other) noexcept {
				vbo = other.vbo;
				sizeInBytes = other.sizeInBytes;
				other.vbo = 0;
			}

			// Move
			VertexBuffer& operator=(VertexBuffer&& other) noexcept {
				vbo = other.vbo;
				sizeInBytes = other.sizeInBytes;
				other.vbo = 0;
				return *this;
			}

			void initialize() { glCreateBuffers(1, &vbo); }

			void load(unsigned int size, float* data) {
				glNamedBufferStorage(vbo, size, data, GL_DYNAMIC_STORAGE_BIT);
				glBindBufferRange(GL_ARRAY_BUFFER, 0, vbo, 0, size);
				sizeInBytes = size;
			};
			void update(unsigned int size, float* data) {
				glNamedBufferSubData(vbo, 0, size, data);
				sizeInBytes = size;
			};
			void bind(unsigned int bindPoint) { glBindBufferRange(GL_ARRAY_BUFFER, bindPoint, vbo, 0, sizeInBytes); };
			void unbind() { glBindBuffer(GL_ARRAY_BUFFER, NULL); };

			GLuint getId() const { return vbo; };

		private:
			GLuint vbo = 0;
			unsigned int sizeInBytes = 0; // size in bytes
		};
	}
}
