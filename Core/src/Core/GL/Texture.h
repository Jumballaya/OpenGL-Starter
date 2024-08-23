#pragma once

#include <string>


#include <glad/glad.h>

namespace Core {
	namespace GL {

		struct TextureOptions {
			GLint maxLevel = 0;
			GLint minFilter = GL_LINEAR;
			GLint maxFilter = GL_LINEAR;
			GLint wrapS = GL_REPEAT;
			GLint wrapT = GL_REPEAT;
			GLint internalFormat = GL_RGBA;
			GLint format = GL_RGBA;
		};
	
		class Texture {
		public:
			Texture();
			~Texture();

			void setup(const char* path, const TextureOptions& opts);
			void setup(uint8_t* data, int width, int height, const TextureOptions& opts);

			void bind(GLuint slot);

			std::string type = "";
			std::string path = "";

			bool loaded = false;
		private:
			GLuint texId;
		
		};

	}
}