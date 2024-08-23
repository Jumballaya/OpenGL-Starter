#include "Texture.h"

#include <stdint.h>
#include <stb/stb_image.h>


namespace Core {
	namespace GL {
	
		Texture::Texture() {
			texId = 0;
		}

		Texture::~Texture() {
			glDeleteTextures(1, &texId);
		}

		void Texture::bind(GLuint slot) {
			glBindTextureUnit(slot, texId);
		}

		void Texture::setup(const char* path, const TextureOptions& opts) {
			if (loaded) return;
			glCreateTextures(GL_TEXTURE_2D, 1, &texId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.wrapT);
			glTextureParameteri(texId, GL_TEXTURE_MAX_LEVEL, opts.maxLevel);
			glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, opts.minFilter);
			glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, opts.maxFilter);
			glGenerateTextureMipmap(texId);

			int w, h, comp;
			const uint8_t* img = stbi_load(path, &w, &h, &comp, 0);

			glTextureStorage2D(texId, 1, opts.internalFormat, w, h);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(texId, 0, 0, 0, w, h, opts.format, GL_UNSIGNED_BYTE, img);
			loaded = true;
		}

		void Texture::setup(uint8_t* data, int width, int height, const TextureOptions& opts) {
			if (loaded) return;
			glCreateTextures(GL_TEXTURE_2D, 1, &texId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.wrapT);
			glTextureParameteri(texId, GL_TEXTURE_MAX_LEVEL, opts.maxLevel);
			glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, opts.minFilter);
			glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, opts.maxFilter);
			glGenerateTextureMipmap(texId);

			glTextureStorage2D(texId, 1, opts.internalFormat, width, height);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(texId, 0, 0, 0, width, height, opts.format, GL_UNSIGNED_BYTE, data);
			loaded = true;
		}
	}
}