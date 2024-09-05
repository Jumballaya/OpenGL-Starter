#pragma once

#include <vector>

#include <glad/glad.h>

#include "Vertexbuffer.h"
#include "ElementBuffer.h"

namespace Core {
	namespace GL {
		struct VertexAttribute {
		public:
			int size;        // number of elements
			int offset;      // offset in bytes
			GLenum type;     // GL_FLOAT or GL_UNSIGNED_BYTE etc
			bool normalized; // is the data normalized?
		};

		class VertexArray {
		public:
			VertexArray() = default;
			~VertexArray() {
				if (vao == 0) return;
				glDeleteVertexArrays(1, &vao);
			};

			// Move
			VertexArray(VertexArray&& other) noexcept {
				vao = other.vao;
				vertex_count = other.vertex_count;
				buffers = std::move(other.buffers);
				elementBuffer = other.elementBuffer;
				other.vao = 0;
			}

			// Move
			VertexArray& operator=(VertexArray&& other) noexcept {
				vao = other.vao;
				vertex_count = other.vertex_count;
				buffers = std::move(other.buffers);
				elementBuffer = other.elementBuffer;
				other.vao = 0;
				return *this;
			}

			void initialize() {
				glCreateVertexArrays(1, &vao);
			}

			void loadVertexBuffer(
				unsigned int size,
				unsigned int stride,
				float* data, std::vector<VertexAttribute> attributes
			) {
				VertexBuffer buffer;
				buffer.initialize();
				buffer.load(size, data);
				int attrId = 0;
				int vertexElems = 0;
				glVertexArrayVertexBuffer(vao, 0, buffer.getId(), 0, stride);
				for (VertexAttribute attr : attributes) {
					glEnableVertexArrayAttrib(vao, attrId);
					glVertexArrayAttribFormat(vao, attrId, attr.size, attr.type, attr.normalized ? GL_TRUE : GL_FALSE, attr.offset);
					glVertexArrayAttribBinding(vao, attrId, 0);
					attrId++;
					vertexElems += attr.size;
				}

				vertex_count = size / vertexElems / 4;
				buffers.push_back(buffer);
			}

			void loadElementBuffer(
				unsigned int size,
				unsigned int* data
			) {
				elementBuffer.initialize();
				elementBuffer.bind(0);
				elementBuffer.load(size, data);
				glVertexArrayElementBuffer(vao, elementBuffer.getId());
			}

			void bind() { glBindVertexArray(vao); }
			void unbind() { glBindVertexArray(0); }

			VertexBuffer& getVertexBuffer(unsigned int id) {
				return buffers[id];
			};

			ElementBuffer getElementBuffer() {
				return elementBuffer;
			};

			GLuint getId() { return vao; }
			unsigned int vertexCount() { return vertex_count; }

		private:
			GLuint vao = 0;
			unsigned int vertex_count = 0;

			std::vector<VertexBuffer> buffers;
			ElementBuffer elementBuffer;
		};
	}
}
