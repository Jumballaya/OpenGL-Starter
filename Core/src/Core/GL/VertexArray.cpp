#include <glad/glad.h>

#include "VertexArray.h"

#include <iostream>

namespace Core {
	namespace GL {
		VertexArray::VertexArray() {
			vertex_count = 0;
			vao = 0;
		}

		void VertexArray::setup() {
			glCreateVertexArrays(1, &vao);
		}

		void VertexArray::destroy() {
			for (auto buff : buffers) {
				if (buff != nullptr) buff->destroy();
			}
			buffers.clear();
			glDeleteVertexArrays(1, &vao);
		}

		void VertexArray::loadVertexBuffer(unsigned int size, unsigned int stride, float* data, std::vector<VertexAttribute> attributes) {
			VertexBuffer* buffer = new VertexBuffer();
			buffer->setup();
			buffer->load(size, data);
			int attrId = 0;
			int vertexElems = 0;
			glVertexArrayVertexBuffer(vao, 0, buffer->getId(), 0, stride);
			for (VertexAttribute attr : attributes) {
				glEnableVertexArrayAttrib(vao, attrId);
				glVertexArrayAttribFormat(vao, attrId, attr.size, GL_FLOAT, GL_FALSE, attr.offset);
				glVertexArrayAttribBinding(vao, attrId, 0);
				attrId++;
				vertexElems += attr.size;
			}

			vertex_count = size / vertexElems / 4;
			buffers.push_back(buffer);
		};

		void VertexArray::loadElementBuffer(unsigned int size, unsigned int* data) {
			elementBuffer.setup();
			elementBuffer.bind(0);
			elementBuffer.load(size, data);
			glVertexArrayElementBuffer(vao, elementBuffer.getId());
		}


		void VertexArray::bind() {
			glBindVertexArray(vao);
		};

		void VertexArray::unbind() {
			glBindVertexArray(0);
		};
	
		unsigned int VertexArray::getId() {
			return vao;
		}
	}
}
