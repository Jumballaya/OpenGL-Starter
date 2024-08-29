#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

namespace Core {
	namespace Render {

		enum BitmapType {
			BitmapType_2D,
			BitmapType_Cube
		};

		enum BitmapFormat {
			BitmapFormat_UnsignedByte,
			BitmapFormat_Float
		};

		class Bitmap {
		public:
			Bitmap() = default;
			~Bitmap() = default;

			void setup() {};
			void load(std::string path, BitmapFormat fmt);
			void load(int w, int h, int comp, BitmapFormat fmt);
			void load(int w, int h, int d, int comp, BitmapFormat fmt);
			void load(int w, int h, int comp, BitmapFormat fmt, const void* ptr);
			void destroy();
			void save(std::string path);

			void setPixel(int x, int y, const glm::vec4& c);
			glm::vec4 getPixel(int x, int y) const;

			int getWidth();
			int getHeight();
			int getComponentCount();
			BitmapFormat getFormat();
			BitmapType getType();
			std::vector<uint8_t>& getData() { return data; };

			void setType(BitmapType typ) { type = typ; }

			static int BytesPerComponent(BitmapFormat format) {
				if (format == BitmapFormat_UnsignedByte) return 1;
				if (format == BitmapFormat_Float) return 4;
			}

			std::vector<uint8_t> data;
		private:
			int width = 0;
			int height = 0;
			int depth = 1;
			int comps = 3;

			BitmapFormat format = BitmapFormat_UnsignedByte;
			BitmapType type = BitmapType_2D;

			// Set/Get pixels
			using setPixel_t = void(Bitmap::*)(int, int, const glm::vec4&);
			using getPixel_t = glm::vec4(Bitmap::*)(int, int) const;
			setPixel_t setPixelFunc = &Bitmap::setPixelUnsignedByte;
			getPixel_t getPixelFunc = &Bitmap::getPixelUnsignedByte;

			void setPixelFloat(int x, int y, const glm::vec4& c);
			void setPixelUnsignedByte(int x, int y, const glm::vec4& c);

			glm::vec4 getPixelFloat(int x, int y) const;
			glm::vec4 getPixelUnsignedByte(int x, int y) const;

			void setupGetSetFuncs();
		};
	}
}