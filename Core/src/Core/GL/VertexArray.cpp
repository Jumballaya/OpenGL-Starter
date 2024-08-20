#include <glad/glad.h>

#include "VertexArray.h"

#include <iostream>

namespace Core {
	namespace GL {
		VertexArray::VertexArray() {
			glGenVertexArrays(1, &vao);
			vertex_count = 0;
		}

		VertexArray::~VertexArray() {
			for (auto buff : buffers) {
				if (buff != NULL) delete buff;
			}
			buffers.clear();
			glDeleteVertexArrays(1, &vao);
		};

		// Add individual buffers
		void VertexArray::addBuffer(unsigned int count, unsigned int size, float* data) {
			int idx = buffers.size();
			bind();
			VertexBuffer* buffer = new VertexBuffer(size, data);
			buffers.push_back(buffer);
			buffer->bind();
			glEnableVertexAttribArray(idx);
			glVertexAttribPointer(idx, count, GL_FLOAT, GL_FALSE, count * sizeof(float), (void*)0);
			buffer->unbind();
			unbind();

			vertex_count = size / count / 4;
		};

		// Add interleved data buffer
		void VertexArray::addBuffer(unsigned int size, float* data, unsigned int stride, std::vector<VertexAttribute> attributes) {
			bind();
			VertexBuffer* buffer = new VertexBuffer(size, data);
			buffers.push_back(buffer);
			buffer->bind();
			int offset = 0;
			int attrId = 0;
			int attrCount = 0;
			for (VertexAttribute attr : attributes) {
				glEnableVertexAttribArray(attrId);
				glVertexAttribPointer(attrId, attr.size, GL_FLOAT, false, stride, (void*)attr.offset);
				offset++;
				attrId++;
				attrCount += attr.size;
			}

			buffer->unbind();
			unbind();

			vertex_count = size / attrCount / 4;
		};


		void VertexArray::bind() {
			glBindVertexArray(vao);
		};

		void VertexArray::unbind() {
			glBindVertexArray(-1);
		};
	
	}
}
