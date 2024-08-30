#include "Bitmap.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <Core/Math/math.h>

namespace Core {
	namespace Render {
		static glm::vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize) {
			const float A = 2.0f * float(i) / faceSize;
			const float B = 2.0f * float(j) / faceSize;

			if (faceID == 0) return glm::vec3(-1.0f, A - 1.0f, B - 1.0f);
			if (faceID == 1) return glm::vec3(A - 1.0f, -1.0f, 1.0f - B);
			if (faceID == 2) return glm::vec3(1.0f, A - 1.0f, 1.0f - B);
			if (faceID == 3) return glm::vec3(1.0f - A, 1.0f, 1.0f - B);
			if (faceID == 4) return glm::vec3(B - 1.0f, A - 1.0f, 1.0f);
			if (faceID == 5) return glm::vec3(1.0f - B, A - 1.0f, -1.0f);

			return glm::vec3();
		}

		static Bitmap convertEquirectangularMapToVerticalCross(Bitmap& b) {
			if (b.getType() != BitmapType_2D) return Bitmap();

			const int faceSize = b.getWidth() / 4;

			const int w = faceSize * 3;
			const int h = faceSize * 4;

			Bitmap result;
			result.load(w, h, b.getComponentCount(), b.getFormat());

			const glm::ivec2 kFaceOffsets[] = {
				glm::ivec2(faceSize, faceSize * 3),
				glm::ivec2(0, faceSize),
				glm::ivec2(faceSize, faceSize),
				glm::ivec2(faceSize * 2, faceSize),
				glm::ivec2(faceSize, 0),
				glm::ivec2(faceSize, faceSize * 2)
			};

			const int clampW = b.getWidth() - 1;
			const int clampH = b.getHeight() - 1;

			for (int face = 0; face != 6; face++)
			{
				for (int i = 0; i != faceSize; i++)
				{
					for (int j = 0; j != faceSize; j++)
					{
						const glm::vec3 P = faceCoordsToXYZ(i, j, face, faceSize);
						const float R = hypot(P.x, P.y);
						const float theta = atan2(P.y, P.x);
						const float phi = atan2(P.z, R);
						//	float point source coordinates
						const float Uf = float(2.0f * faceSize * (theta + M_PI) / M_PI);
						const float Vf = float(2.0f * faceSize * (M_PI / 2.0f - phi) / M_PI);
						// 4-samples for bilinear interpolation
						const int U1 = Math::clamp(int(floor(Uf)), 0, clampW);
						const int V1 = Math::clamp(int(floor(Vf)), 0, clampH);
						const int U2 = Math::clamp(U1 + 1, 0, clampW);
						const int V2 = Math::clamp(V1 + 1, 0, clampH);
						// fractional part
						const float s = Uf - U1;
						const float t = Vf - V1;
						// fetch 4-samples
						const glm::vec4 A = b.getPixel(U1, V1);
						const glm::vec4 B = b.getPixel(U2, V1);
						const glm::vec4 C = b.getPixel(U1, V2);
						const glm::vec4 D = b.getPixel(U2, V2);
						// bilinear interpolation
						const glm::vec4 color = A * (1 - s) * (1 - t) + B * (s) * (1 - t) + C * (1 - s) * t + D * (s) * (t);
						result.setPixel(i + kFaceOffsets[face].x, j + kFaceOffsets[face].y, color);
					}
				};
			}

			return result;
		}

		static Bitmap convertVerticalCrossToCubeMapFaces(Bitmap& b) {
			const int faceWidth = b.getWidth() / 3;
			const int faceHeight = b.getHeight() / 4;

			Bitmap cubemap;
			cubemap.setType(BitmapType_Cube);
			cubemap.load(faceWidth, faceHeight, 6, b.getComponentCount(), b.getFormat());

			const uint8_t* src = b.getData().data();
			uint8_t* dst = cubemap.getData().data();

			const int pixelSize = cubemap.getComponentCount() * Bitmap::BytesPerComponent(cubemap.getFormat());

			for (int face = 0; face < 6; ++face) {
				for (int j = 0; j < faceHeight; ++j) {
					for (int i = 0; i < faceWidth; ++i) {
						int x = 0;
						int y = 0;

						switch (face) {
							// GL_TEXTURE_CUBE_MAP_POSITIVE_X
						case 0:
							x = i;
							y = faceHeight + j;
							break;

							// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
						case 1:
							x = 2 * faceWidth + i;
							y = 1 * faceHeight + j;
							break;

							// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
						case 2:
							x = 2 * faceWidth - (i + 1);
							y = 1 * faceHeight - (j + 1);
							break;

							// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
						case 3:
							x = 2 * faceWidth - (i + 1);
							y = 3 * faceHeight - (j + 1);
							break;

							// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
						case 4:
							x = 2 * faceWidth - (i + 1);
							y = b.getHeight() - (j + 1);
							break;

							// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
						case 5:
							x = faceWidth + i;
							y = faceHeight + j;
							break;
						}

						memcpy(dst, src + (y * b.getWidth() + x) * pixelSize, pixelSize);
						dst += pixelSize;
					}
				}
			}

			return cubemap;
		}

		Bitmap Bitmap::GenerateCubeMap(std::string path, BitmapFormat format) {
			Core::Render::Bitmap envMap;
			envMap.setup();
			envMap.load(path, format);
			Core::Render::Bitmap cross = convertEquirectangularMapToVerticalCross(envMap);
			Core::Render::Bitmap cubemap = convertVerticalCrossToCubeMapFaces(cross);
			envMap.destroy();
			cross.destroy();
			return cubemap;
		}

		void Bitmap::load(std::string path, BitmapFormat fmt) {
			int w, h, comp;
			const void* img;
			if (fmt == BitmapFormat_Float) {
				img = stbi_loadf(path.c_str(), &w, &h, &comp, 3);
			} else {
				img = stbi_load(path.c_str(), &w, &h, &comp, 0);
			}
			width = w;
			height = h;
			comps = comp;
			format = fmt;
			data = std::vector<uint8_t>(w * h * BytesPerComponent(fmt) * comp);
			memcpy(data.data(), img, data.size());
			setupGetSetFuncs();
		}

		void Bitmap::load(int w, int h, int comp, BitmapFormat fmt)  {
			width = w;
			height = h;
			comps = comp;
			format = fmt;
			data = std::vector<uint8_t>(w * h * BytesPerComponent(fmt) * comp);
			setupGetSetFuncs();
		}

		void Bitmap::load(int w, int h, int d, int comp, BitmapFormat fmt) {
			width = w;
			height = h;
			depth = d;
			comps = comp;
			format = fmt;
			data = std::vector<uint8_t>(w * h * d * BytesPerComponent(fmt) * comp);
			setupGetSetFuncs();
		}

		void Bitmap::load(int w, int h, int comp, BitmapFormat fmt, const void* ptr) {
			width = w;
			height = h;
			comps = comp;
			format = fmt;
			data = std::vector<uint8_t>(w * h * BytesPerComponent(fmt) * comp);
			memcpy(data.data(), ptr, data.size());
			setupGetSetFuncs();
		}

		void Bitmap::destroy() {
			data.clear();
		}

		void Bitmap::save(std::string path) {
			std::string filename = path.substr(path.find_last_of('/')+1, path.size());
			std::string extension = filename.substr(filename.find_last_of('.') + 1, filename.size());
			if (format == BitmapFormat_Float) {
				stbi_write_hdr(path.c_str(), width, height, comps, reinterpret_cast<const float*>(data.data()));
				return;
			} else if (extension.compare("png")) {
				stbi_write_png(path.c_str(), width, height, comps, data.data(), width * comps);
				return;
			} else if (extension.compare("jpg") || extension.compare("jpeg")) {
				stbi_write_jpg(path.c_str(), width, height, comps, data.data(), 100);
				return;
			}
		}

		int Bitmap::getWidth() { return width; };
		int Bitmap::getHeight() { return height; };
		int Bitmap::getComponentCount() { return comps; }
		BitmapFormat Bitmap::getFormat() { return format; }
		BitmapType Bitmap::getType() { return type; }

		void Bitmap::setPixel(int x, int y, const glm::vec4& c) {
			(*this.*setPixelFunc)(x, y, c);
		}

		glm::vec4 Bitmap::getPixel(int x, int y) const {
			return (*this.*getPixelFunc)(x, y);
		}

		void Bitmap::setupGetSetFuncs() {
			switch (format) {
			case BitmapFormat_Float:
				setPixelFunc = &Bitmap::setPixelFloat;
				getPixelFunc = &Bitmap::getPixelFloat;
				break;
			case BitmapFormat_UnsignedByte:
				setPixelFunc = &Bitmap::setPixelUnsignedByte;
				getPixelFunc = &Bitmap::getPixelUnsignedByte;
			}
		}

		void Bitmap::setPixelUnsignedByte(int x, int y, const glm::vec4& c) {
			const int ofs = comps * (y * width + x);
			if (comps > 0) data[ofs + 0] = uint8_t(c.x * 255.0f);
			if (comps > 1) data[ofs + 1] = uint8_t(c.y * 255.0f);
			if (comps > 2) data[ofs + 2] = uint8_t(c.z * 255.0f);
			if (comps > 3) data[ofs + 3] = uint8_t(c.w * 255.0f);
		}

		glm::vec4 Bitmap::getPixelUnsignedByte(int x, int y) const {
			const int ofs = comps * (y * width + x);
			return glm::vec4(
				comps > 0 ? float(data[ofs + 0]) / 255.0f : 0.0f,
				comps > 1 ? float(data[ofs + 1]) / 255.0f : 0.0f,
				comps > 2 ? float(data[ofs + 2]) / 255.0f : 0.0f,
				comps > 3 ? float(data[ofs + 3]) / 255.0f : 0.0f);
		}

		void Bitmap::setPixelFloat(int x, int y, const glm::vec4& c) {
			const int ofs = comps * (y * width + x);
			float* _data = reinterpret_cast<float*>(data.data());
			if (comps > 0) _data[ofs + 0] = c.x;
			if (comps > 1) _data[ofs + 1] = c.y;
			if (comps > 2) _data[ofs + 2] = c.z;
			if (comps > 3) _data[ofs + 3] = c.w;
		}

		glm::vec4 Bitmap::getPixelFloat(int x, int y) const {
			const int ofs = comps * (y * width + x);
			const float* _data = reinterpret_cast<const float*>(data.data());
			return glm::vec4(
				comps > 0 ? _data[ofs + 0] : 0.0f,
				comps > 1 ? _data[ofs + 1] : 0.0f,
				comps > 2 ? _data[ofs + 2] : 0.0f,
				comps > 3 ? _data[ofs + 3] : 0.0f);
		}


	}
}