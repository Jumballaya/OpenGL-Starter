#include "App.h"

#include <glm/glm.hpp>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

using Core::Render::Bitmap;
using Core::Math::clamp;
using glm::ivec2;
using glm::vec3;
using glm::vec4;
vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize) {
	const float A = 2.0f * float(i) / faceSize;
	const float B = 2.0f * float(j) / faceSize;

	if (faceID == 0) return vec3(-1.0f, A - 1.0f, B - 1.0f);
	if (faceID == 1) return vec3(A - 1.0f, -1.0f, 1.0f - B);
	if (faceID == 2) return vec3(1.0f, A - 1.0f, 1.0f - B);
	if (faceID == 3) return vec3(1.0f - A, 1.0f, 1.0f - B);
	if (faceID == 4) return vec3(B - 1.0f, A - 1.0f, 1.0f);
	if (faceID == 5) return vec3(1.0f - B, A - 1.0f, -1.0f);

	return vec3();
}

Bitmap convertEquirectangularMapToVerticalCross(Bitmap& b)
{
	if (b.getType() != Core::Render::BitmapType_2D) return Bitmap();

	const int faceSize = b.getWidth() / 4;

	const int w = faceSize * 3;
	const int h = faceSize * 4;

	Bitmap result;
	result.load(w, h, b.getComponentCount(), b.getFormat());

	const ivec2 kFaceOffsets[] = {
		ivec2(faceSize, faceSize * 3),
		ivec2(0, faceSize),
		ivec2(faceSize, faceSize),
		ivec2(faceSize * 2, faceSize),
		ivec2(faceSize, 0),
		ivec2(faceSize, faceSize * 2)
	};

	const int clampW = b.getWidth() - 1;
	const int clampH = b.getHeight()  - 1;

	for (int face = 0; face != 6; face++)
	{
		for (int i = 0; i != faceSize; i++)
		{
			for (int j = 0; j != faceSize; j++)
			{
				const vec3 P = faceCoordsToXYZ(i, j, face, faceSize);
				const float R = hypot(P.x, P.y);
				const float theta = atan2(P.y, P.x);
				const float phi = atan2(P.z, R);
				//	float point source coordinates
				const float Uf = float(2.0f * faceSize * (theta + M_PI) / M_PI);
				const float Vf = float(2.0f * faceSize * (M_PI / 2.0f - phi) / M_PI);
				// 4-samples for bilinear interpolation
				const int U1 = clamp(int(floor(Uf)), 0, clampW);
				const int V1 = clamp(int(floor(Vf)), 0, clampH);
				const int U2 = clamp(U1 + 1, 0, clampW);
				const int V2 = clamp(V1 + 1, 0, clampH);
				// fractional part
				const float s = Uf - U1;
				const float t = Vf - V1;
				// fetch 4-samples
				const vec4 A = b.getPixel(U1, V1);
				const vec4 B = b.getPixel(U2, V1);
				const vec4 C = b.getPixel(U1, V2);
				const vec4 D = b.getPixel(U2, V2);
				// bilinear interpolation
				const vec4 color = A * (1 - s) * (1 - t) + B * (s) * (1 - t) + C * (1 - s) * t + D * (s) * (t);
				result.setPixel(i + kFaceOffsets[face].x, j + kFaceOffsets[face].y, color);
			}
		};
	}

	return result;
}

Bitmap convertVerticalCrossToCubeMapFaces(Bitmap& b) {
	const int faceWidth = b.getWidth() / 3;
	const int faceHeight = b.getHeight() / 4;

	Bitmap cubemap;
	cubemap.setType(Core::Render::BitmapType_Cube);
	cubemap.load(faceWidth, faceHeight, 6, b.getComponentCount(), b.getFormat());

	const std::vector<uint8_t> bitmapData = b.getData();
	std::vector<uint8_t> cubemapData = cubemap.getData();
	const uint8_t* src = bitmapData.data();
	uint8_t* dst = cubemapData.data();
	const int pixelSize = cubemap.getComponentCount() * Bitmap::BytesPerComponent(cubemap.getFormat());

	for (int face = 0; face != 6; ++face) {
		for (int j = 0; j != faceHeight; ++j) {
			for (int i = 0; i != faceWidth; ++i) {
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


int main() {
	App app;
	Entity ent;
	ent.setup();
	ent.loadShader("shaders/model/vertex.glsl", "shaders/model/fragment.glsl");
	ent.loadModel("models/duck/Duck.gltf");

	ent.transform.scale(glm::vec3(0.05f));
	ent.transform.translate(glm::vec3(0.0f, -4.0f, -12.0f));

	Core::Render::Bitmap envMap;
	envMap.load("textures/piazza_bologni_1k.hdr", Core::Render::BitmapFormat_Float);
	Core::Render::Bitmap cross = convertEquirectangularMapToVerticalCross(envMap);
	//Core::Render::Bitmap cubemap = convertVerticalCrossToCubeMapFaces(cross);
	//cubemap.save("textures/cubemap.hdr");

	Core::GL::Texture crossTexture;
	crossTexture.setup();
	crossTexture.load(cross, Core::GL::TextureOptions::HDR());

	//GLuint cubemapTex;
	//glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &cubemapTex);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_BASE_LEVEL, 0);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_MAX_LEVEL, 0);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTextureParameteri(cubemapTex, GL_TEXTURE_CUBE_MAP_SEAMLESS, GL_TRUE);
	//glTextureStorage2D(cubemapTex, 1, GL_RGB32F, cross.getWidth(), cross.getHeight());
	//std::vector<uint8_t> cubeData = cross.getData();
	//const uint8_t* data = cubeData.data();

	//for (unsigned int i = 0; i < 6; ++i) {
	//	glTextureSubImage3D(cubemapTex, 0, 0, 0, i, cross.getWidth(), cross.getHeight(), 1, GL_RGB, GL_FLOAT, data);
	//	data += cross.getWidth() * cross.getHeight() * cross.getComponentCount() * Core::Render::Bitmap::BytesPerComponent(cross.getFormat());
	//}

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	float spin = 0.0f;
	app.run([&ent, &spin, &crossTexture](App* app, int width, int height) {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		ent.transform.rotateY(spin);
		crossTexture.bind(5);
		ent.draw(app->camera);
		spin += 3.1415f / 180.0f;
		app->gui.drawTextureWindowGL("Duck Texture", ent.model.meshes[0].textures[0].getId());
		app->gui.drawTextureWindowGL("Cross Texture", crossTexture.getId());
	});

	ent.destroy();
	app.destroy();

	exit(EXIT_SUCCESS);
}
