#include "Bitmap.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

namespace Core {
	namespace Render {
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