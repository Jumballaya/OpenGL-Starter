#pragma once

namespace Core {
	namespace GL {
		class VertexBuffer {
		public:
			VertexBuffer(unsigned int size, float* data);
			~VertexBuffer();

			void bind();
			void unbind();

		private:
			unsigned int vbo;
		};
	}
}
