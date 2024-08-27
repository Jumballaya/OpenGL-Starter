#pragma once

#include <vector>
#include <iostream>

#include "Vertexbuffer.h"
#include "ElementBuffer.h"

namespace Core {
	namespace GL {
		struct VertexAttribute {
			int size;   // number of elements
			int offset; // offset in bytes
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
