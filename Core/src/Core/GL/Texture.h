#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>
#include "Core/Render/Bitmap.h"

namespace Core {
	namespace GL {

		struct TextureOptions {
		public:
			GLint maxLevel = 0;
			GLint baseLevel = 0;
			GLint minFilter = GL_LINEAR;
			GLint magFilter = GL_LINEAR;
			GLint wrapS = GL_CLAMP_TO_EDGE;
			GLint wrapT = GL_CLAMP_TO_EDGE;
			GLint wrapR = GL_CLAMP_TO_EDGE;
			GLint internalFormat = GL_RGBA8;
			GLint format = GL_RGBA;
			GLint type = GL_UNSIGNED_BYTE;
			GLint textureType = GL_TEXTURE_2D;

			static TextureOptions HDR() {
				TextureOptions opts;
				opts.format = GL_RGB;
				opts.type = GL_FLOAT;
				opts.internalFormat = GL_RGB32F;
				opts.wrapS = GL_CLAMP_TO_EDGE;
				opts.wrapT = GL_CLAMP_TO_EDGE;
				opts.wrapS = GL_CLAMP_TO_EDGE;
				opts.maxLevel = 0;
				opts.baseLevel = 0;
				opts.minFilter = GL_LINEAR;
				opts.magFilter = GL_LINEAR;
				return opts;
			}

			static TextureOptions CubeMap() {
				TextureOptions opts;
				opts.textureType = GL_TEXTURE_CUBE_MAP;
				opts.format = GL_RGBA;
				opts.type = GL_UNSIGNED_BYTE;
				opts.internalFormat = GL_RGBA8;
				opts.wrapS = GL_CLAMP_TO_EDGE;
				opts.wrapT = GL_CLAMP_TO_EDGE;
				opts.wrapS = GL_CLAMP_TO_EDGE;
				opts.maxLevel = 0;
				opts.baseLevel = 0;
				opts.minFilter = GL_LINEAR;
				opts.magFilter = GL_LINEAR;
				return opts;
			}

			static TextureOptions CubeMapf() {
				TextureOptions opts;
				opts.textureType = GL_TEXTURE_CUBE_MAP;
				opts.format = GL_RGB;
				opts.type = GL_FLOAT;
				opts.internalFormat = GL_RGB32F;
				opts.wrapS = GL_CLAMP_TO_EDGE;
				opts.wrapT = GL_CLAMP_TO_EDGE;
				opts.wrapS = GL_CLAMP_TO_EDGE;
				opts.maxLevel = 0;
				opts.baseLevel = 0;
				opts.minFilter = GL_LINEAR;
				opts.magFilter = GL_LINEAR;
				return opts;
			}
		};
	
		class Texture {
		public:
			Texture();
			~Texture() = default;

			void setup();
			void load(Render::Bitmap& bitmap, const TextureOptions& opts);
			void load(const char* path, const TextureOptions& opts);
			void load(uint8_t* data, int width, int height, const TextureOptions& opts);
			void destroy();

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