#pragma once

#include <glad/glad.h>

namespace Core {
	namespace GL {

		struct SamplerOptions {
		public:
			GLint maxLevel = 0;
			GLint baseLevel = 0;
			GLint minFilter = GL_LINEAR;
			GLint magFilter = GL_LINEAR;
			GLint wrapS = GL_CLAMP_TO_EDGE;
			GLint wrapT = GL_CLAMP_TO_EDGE;
			GLint wrapR = GL_CLAMP_TO_EDGE;
		};

		class Sampler {
		public:
			Sampler() : options(SamplerOptions()) {};
			~Sampler() {
				if (sampler == 0) return;
				glDeleteSamplers(1, &sampler);
			};

			// Move
			Sampler(Sampler&& other) noexcept {
				sampler = other.sampler;
				other.sampler = 0;
			}

			// Move
			Sampler& operator=(Sampler&& other) noexcept {
				sampler = other.sampler;
				other.sampler = 0;
				return *this;
			}

			void initialize() { setOptions(); }
			void load(const SamplerOptions& opts) {
				options = opts;
				setOptions();
			};

			void bind(GLuint unit) { glBindSampler(unit, sampler); }
			void unbind() {}; // API completeness for now

			GLuint getId() const { return sampler; };

		private:
			GLuint sampler = 0;
			SamplerOptions options;

			void setOptions() {
				glCreateSamplers(1, &sampler);
				glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, options.wrapS);
				glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, options.wrapT);
				glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, options.wrapR);
				glSamplerParameteri(sampler, GL_TEXTURE_MAX_LEVEL, options.maxLevel);
				glSamplerParameteri(sampler, GL_TEXTURE_BASE_LEVEL, options.baseLevel);
				glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, options.minFilter);
				glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, options.magFilter);
			}
		};

	}
}