#pragma once

#include <glad/glad.h>

#include "Core/Render/Bitmap.h"

namespace Core {
	namespace GL {

		struct TextureOptions {
		public:
			GLint internalFormat = GL_RGBA8;
			GLint format = GL_RGBA;
			GLint type = GL_UNSIGNED_BYTE;
			GLint textureType = GL_TEXTURE_2D;

			static TextureOptions HDR() {
				TextureOptions opts;
				opts.format = GL_RGB;
				opts.type = GL_FLOAT;
				opts.internalFormat = GL_RGB32F;
				return opts;
			}

			static TextureOptions CubeMap(unsigned int channels = 3) {
				TextureOptions opts;
				opts.textureType = GL_TEXTURE_CUBE_MAP;
				opts.type = GL_UNSIGNED_BYTE;
				switch (channels) {
				case 4:
					opts.format = GL_RGBA;
					opts.internalFormat = GL_RGBA8;
					break;
				case 3:
					opts.format = GL_RGB;
					opts.internalFormat = GL_RGB8;
					break;
				default:
					opts.format = GL_RED;
					opts.internalFormat = GL_R8;
					break;
				}
				return opts;
			}

			static TextureOptions CubeMapf(unsigned int channels = 3) {
				TextureOptions opts;
				opts.textureType = GL_TEXTURE_CUBE_MAP;
				opts.type = GL_FLOAT;
				switch (channels) {
				case 4:
					opts.format = GL_RGBA;
					opts.internalFormat = GL_RGBA32F;
					break;
				case 3:
					opts.format = GL_RGB;
					opts.internalFormat = GL_RGB32F;
					break;
				default:
					opts.format = GL_RED;
					opts.internalFormat = GL_R32F;
					break;
				}
				return opts;
			}
		};
	
		class Texture {
		public:
			Texture() = default;
			~Texture() {
				if (texId == 0) return;
				glDeleteTextures(1, &texId);
			};

			void initialize() {}; // API completeness
			void load(Render::Bitmap& bitmap, const TextureOptions& opts) {
				if (loaded) return;
				std::vector<uint8_t> imgData = bitmap.getData();
				uint8_t* img = imgData.data();
				int w = bitmap.getWidth();
				int h = bitmap.getHeight();
				int comps = bitmap.getComponentCount();
				load(img, w, h, opts);
			}

			void load(const char* path, const TextureOptions& opts) {
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

			void load(uint8_t* img, int width, int height, const TextureOptions& opts) {
				if (loaded) return;
				glCreateTextures(opts.textureType, 1, &texId);
				glGenerateTextureMipmap(texId);

				glTextureStorage2D(texId, 1, opts.internalFormat, width, height);

				if (opts.textureType == GL_TEXTURE_CUBE_MAP) {
					int comps = 3;                                 // TODO: get the component count in this function
					int byteCount = opts.type == GL_FLOAT ? 4 : 1; // TODO: get byte count by type
					for (unsigned i = 0; i < 6; ++i) {
						glTextureSubImage3D(texId, 0, 0, 0, i, width, height, 1, opts.format, opts.type, img);
						img += width * height * comps * byteCount;
					}
					glTextureParameteri(texId, GL_TEXTURE_CUBE_MAP_SEAMLESS, GL_TRUE);
				}
				if (opts.textureType == GL_TEXTURE_2D) {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
					glTextureSubImage2D(texId, 0, 0, 0, width, height, opts.format, opts.type, img);
				}
				loaded = true;
			}

			void bind(GLuint slot) { glBindTextureUnit(slot, texId); }
			void unbind(GLuint slot) { glBindTextureUnit(slot, 0); }

			GLuint getId() { return texId; }

		private:
			GLuint texId = 0;
			bool loaded = false;
		
		};

	}
}