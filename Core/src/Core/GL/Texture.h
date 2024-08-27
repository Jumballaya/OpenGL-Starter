#pragma once

#include <string>
#include <iostream>


#include <glad/glad.h>

namespace Core {
	namespace GL {

		struct TextureOptions {
			GLint maxLevel = 0;
			GLint minFilter = GL_LINEAR;
			GLint maxFilter = GL_LINEAR;
			GLint wrapS = GL_CLAMP_TO_EDGE;
			GLint wrapT = GL_CLAMP_TO_EDGE;
			GLint internalFormat = GL_RGBA;
			GLint format = GL_RGBA;
		};
	
		class Texture {
		public:
			Texture();
			~Texture();

			void setup();

			void load(const char* path, const TextureOptions& opts);
			void load(uint8_t* data, int width, int height, const TextureOptions& opts);

			void bind(GLuint slot);
			void unbind(GLuint slot);

			GLuint getId() { return texId; }

			std::string name = ""; // uniform name
			std::string path = "";
			std::string fileName = "";

			bool loaded = false;
		private:
			GLuint texId = 0;
		
		};

	}
}