#pragma once

#include <iostream>

namespace Core {
	namespace GL {
		class VertexBuffer {
		public:
			VertexBuffer() = default;
			~VertexBuffer() = default;

			void setup();
			void load(unsigned int size, float* data);
			void update(unsigned int size, float* data);
			void destroy();

			unsigned int getId();

			void bind(unsigned int bindPoint);
			void unbind();

		private:
			unsigned int vbo;
			unsigned int size = 0;
		};
	}
}
