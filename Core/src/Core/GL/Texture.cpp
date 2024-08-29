#include "Texture.h"

#include <stdint.h>
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
			Render::Bitmap image;
			image.load(path, opts.type == GL_FLOAT ? Render::BitmapFormat_Float : Render::BitmapFormat_UnsignedByte);

			int w = image.getWidth();
			int h = image.getHeight();
			int comps = image.getComponentCount();

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

			load(image, _opts);

			loaded = true;
		}

		void Texture::load(uint8_t* img, int w, int h, const TextureOptions& opts) {
			if (loaded) return;
			glCreateTextures(opts.textureType, 1, &texId);
			glTextureParameteri(texId, GL_TEXTURE_WRAP_S, opts.wrapS);
			glTextureParameteri(texId, GL_TEXTURE_WRAP_T, opts.wrapT);
			glTextureParameteri(texId, GL_TEXTURE_WRAP_R, opts.wrapR);
			glTextureParameteri(texId, GL_TEXTURE_MAX_LEVEL, opts.maxLevel);
			glTextureParameteri(texId, GL_TEXTURE_BASE_LEVEL, opts.baseLevel);
			glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, opts.minFilter);
			glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, opts.magFilter);
			glGenerateTextureMipmap(texId);

			glTextureStorage2D(texId, 1, opts.internalFormat, w, h);

			if (opts.textureType == GL_TEXTURE_CUBE_MAP) {
				int comps = 3;                                 // TODO: get the component count in this function
				int byteCount = opts.type == GL_FLOAT ? 4 : 1; // TODO: get byte count by type
				for (unsigned i = 0; i < 6; ++i) {
					glTextureSubImage3D(texId, 0, 0, 0, i, w, h, 1, opts.format, opts.type, img);
					img += w * h * comps * byteCount;
				}
			}
			if (opts.textureType == GL_TEXTURE_2D) {
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTextureSubImage2D(texId, 0, 0, 0, w, h, opts.format, opts.type, img);
			}
			loaded = true;
		}

		void Texture::load(Render::Bitmap& bitmap, const TextureOptions& opts) {
			if (loaded) return;
			std::vector<uint8_t> imgData = bitmap.getData();
			uint8_t* img = imgData.data();
			int w = bitmap.getWidth();
			int h = bitmap.getHeight();
			int comps = bitmap.getComponentCount();
			load(img, w, h, opts);
		}

	}
}