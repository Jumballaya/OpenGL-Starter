#include "Texture.h"

#include <stdint.h>
#include <stb/stb_image.h>

#include <iostream>

namespace Core {
	namespace GL {
	
		Texture::Texture() {
			texId = 0;
		}

		void Texture::bind(GLuint slot) {
			if (!loaded) return;
			glBindTextureUnit(slot, texId);
		}

		void Texture::unbind(GLuint slot) {
			if (!loaded) return;
			glBindTextureUnit(slot, 0);
		}

		void Texture::setup() {
			glCreateTextures(GL_TEXTURE_2D, 1, &texId);
		}

		void Texture::destroy() {
			glDeleteTextures(1, &texId);
		}

		void Texture::load(const char* path, const TextureOptions& opts) {
			if (loaded) return;
			TextureOptions _opts = opts;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.wrapT);
			glTextureParameteri(texId, GL_TEXTURE_MAX_LEVEL, opts.maxLevel);
			glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, opts.minFilter);
			glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, opts.maxFilter);
			glGenerateTextureMipmap(texId);

			int w, h, comps;
			uint8_t* img = stbi_load(path, &w, &h, &comps, 0);

			GLenum format = GL_RGBA;
			GLenum internalFormat = GL_RGBA8;
			if (comps == 1) {
				_opts.format = GL_RED;
				_opts.internalFormat = GL_R8;
			}
			else if (comps == 3) {
				_opts.format = GL_RGB;
				_opts.internalFormat = GL_RGB8;
			}
			else if (comps == 4) {
				_opts.format = GL_RGBA;
				_opts.internalFormat = GL_RGBA8;
			}

			glTextureStorage2D(texId, 1, _opts.internalFormat, w, h);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(texId, 0, 0, 0, w, h, _opts.format, GL_UNSIGNED_BYTE, img);
			stbi_image_free(img);
			loaded = true;
		}

		void Texture::load(uint8_t* img, int w, int h, const TextureOptions& opts) {
			if (loaded) return;
			glCreateTextures(GL_TEXTURE_2D, 1, &texId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.wrapT);
			glTextureParameteri(texId, GL_TEXTURE_MAX_LEVEL, opts.maxLevel);
			glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, opts.minFilter);
			glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, opts.maxFilter);
			glGenerateTextureMipmap(texId);

			glTextureStorage2D(texId, 1, opts.internalFormat, w, h);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(texId, 0, 0, 0, w, h, opts.format, GL_UNSIGNED_BYTE, img);
			loaded = true;
		}
	}
}