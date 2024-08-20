#pragma once

#include <vector>

#include "Vertexbuffer.h"

namespace Core {
	namespace GL {
		struct VertexAttribute {
			int size;   // number of elements
			int offset; // offset in bytes
		};

		class VertexArray {
		public:
			VertexArray();
			~VertexArray();

			void addBuffer(unsigned int count, unsigned int size, float* data);
			void addBuffer(unsigned int size, float* data, unsigned int stride, std::vector<VertexAttribute> attributes);

			void bind();
			void unbind();

			unsigned int vertexCount() { return vertex_count; }

		private:
			unsigned int vao;
			unsigned int vertex_count;

			std::vector<VertexBuffer*> buffers;
		};
	}
}
