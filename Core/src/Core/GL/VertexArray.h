#pragma once

#include <vector>
#include <iostream>

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
			VertexArray();
			~VertexArray() = default;

			void loadVertexBuffer(unsigned int size, unsigned int stride, float* data, std::vector<VertexAttribute> attributes);
			void loadElementBuffer(unsigned int size, unsigned int* data);

			unsigned int getId();
			void setup();
			void destroy();

			VertexBuffer getVertexBuffer(unsigned int id);
			ElementBuffer getElementBuffer();

			void bind();
			void unbind();

			unsigned int vertexCount() { return vertex_count; }

		private:
			unsigned int vao;
			unsigned int vertex_count;

			std::vector<VertexBuffer*> buffers;
			ElementBuffer elementBuffer;
		};
	}
}
