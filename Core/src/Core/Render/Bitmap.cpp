#include "Bitmap.h"

namespace Core {
	namespace Render {
		void Bitmap::load(int w, int h, int comp, BitmapFormat fmt) {
			width = w;
			height = h;
			comps = comp;
			format = fmt;
			setupGetSetFuncs();
		}

		void Bitmap::load(int w, int h, int d, int comp, BitmapFormat fmt) {
			width = w;
			height = h;
			depth = d;
			comps = comp;
			format = fmt;
			setupGetSetFuncs();
		}

		void Bitmap::load(int w, int h, int comp, BitmapFormat fmt, const void* ptr) {
			width = w;
			height = h;
			comps = comp;
			format = fmt;
			setupGetSetFuncs();
		}

		void Bitmap::destroy() {
			data.clear();
		}

		void Bitmap::save() {

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
			const int offset = comps * (y * width + x);
			if (comps > 0) data[offset + 0] = uint8_t(c.x * 255.0f);
			if (comps > 1) data[offset + 1] = uint8_t(c.y * 255.0f);
			if (comps > 2) data[offset + 2] = uint8_t(c.z * 255.0f);
			if (comps > 3) data[offset + 3] = uint8_t(c.w * 255.0f);
		}

		glm::vec4 Bitmap::getPixelUnsignedByte(int x, int y) const {
			const int offset = comps * (y * width + x);
			return glm::vec4(
				comps > 0 ? float(data[offset + 0]) / 255.0f : 0.0f,
				comps > 1 ? float(data[offset + 1]) / 255.0f : 0.0f,
				comps > 2 ? float(data[offset + 2]) / 255.0f : 0.0f,
				comps > 3 ? float(data[offset + 3]) / 255.0f : 0.0f);
		}

		void Bitmap::setPixelFloat(int x, int y, const glm::vec4& c) {
			const int offset = comps * (y * width + x);
			if (comps > 0) data[offset + 0] = c.x;
			if (comps > 1) data[offset + 1] = c.y;
			if (comps > 2) data[offset + 2] = c.z;
			if (comps > 3) data[offset + 3] = c.w;
		}

		glm::vec4 Bitmap::getPixelFloat(int x, int y) const {
			const int offset = comps * (y * width + x);
			return glm::vec4(
				comps > 0 ? data[offset + 0] : 0.0f,
				comps > 1 ? data[offset + 1] : 0.0f,
				comps > 2 ? data[offset + 2] : 0.0f,
				comps > 3 ? data[offset + 3] : 0.0f);
		}


	}
}