#pragma once

#include <glad/glad.h>

namespace Core {
	namespace GL {
		class FrameBuffer {
		public:
			FrameBuffer() = default;
			~FrameBuffer() { if (fbo > 0) glDeleteFramebuffers(1, &fbo; }

			// Move
			FrameBuffer(FrameBuffer&& other) noexcept {
				fbo = other.fbo;
				other.fbo = 0;
			}

			// Move
			FrameBuffer& operator=(FrameBuffer&& other) noexcept {
				fbo = other.fbo;
				other.fbo = 0;
				return *this;
			}

			void initialize() { glCreateFramebuffers(1, &fbo); }
			void bind() { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
			void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

		private:
			GLuint fbo;
		};
	}
}