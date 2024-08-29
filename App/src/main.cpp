#include "App.h"

#include <glm/glm.hpp>

glm::vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize) {
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

Core::Render::Bitmap convertEquirectangularMapToVerticalCross(Core::Render::Bitmap& b)
{
	if (b.getType() != Core::Render::BitmapType_2D) return Core::Render::Bitmap();

	const int faceSize = b.getWidth() / 4;

	const int w = faceSize * 3;
	const int h = faceSize * 4;

	Core::Render::Bitmap result;
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
	const int clampH = b.getHeight()  - 1;

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
				const int U1 = Core::Math::clamp(int(floor(Uf)), 0, clampW);
				const int V1 = Core::Math::clamp(int(floor(Vf)), 0, clampH);
				const int U2 = Core::Math::clamp(U1 + 1, 0, clampW);
				const int V2 = Core::Math::clamp(V1 + 1, 0, clampH);
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

Core::Render::Bitmap convertVerticalCrossToCubeMapFaces(Core::Render::Bitmap& b)
{
	const int faceWidth = b.getWidth() / 3;
	const int faceHeight = b.getHeight() / 4;

	Core::Render::Bitmap cubemap;
	cubemap.setType(Core::Render::BitmapType_Cube);
	cubemap.load(faceWidth, faceHeight, 6, b.getComponentCount(), b.getFormat());

	const uint8_t* src = b.getData().data();
	uint8_t* dst = cubemap.getData().data();

	const int pixelSize = cubemap.getComponentCount() * Core::Render::Bitmap::BytesPerComponent(cubemap.getFormat());

	for (int face = 0; face != 6; ++face)
	{
		for (int j = 0; j != faceHeight; ++j)
		{
			for (int i = 0; i != faceWidth; ++i)
			{
				int x = 0;
				int y = 0;

				switch (face)
				{
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
	ent.transform.translate(glm::vec3(0.0f, -4.5f, -10.0f));

	Core::Render::Bitmap envMap;
	envMap.load("textures/piazza_bologni_1k.hdr", Core::Render::BitmapFormat_Float);
	Core::Render::Bitmap cross = convertEquirectangularMapToVerticalCross(envMap);
	Core::Render::Bitmap cubemap = convertVerticalCrossToCubeMapFaces(cross);

	Core::GL::Texture crossTexture;
	crossTexture.setup();
	crossTexture.load(cross, Core::GL::TextureOptions::HDR());

	Core::GL::Texture cubemapTex;
	cubemapTex.setup();
	cubemapTex.load(cubemap, Core::GL::TextureOptions::CubeMapf());

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	float spin = 0.0f;
	app.run([&ent, &spin, &crossTexture, &cubemapTex](App* app, int width, int height) {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		cubemapTex.bind(1);
		ent.transform.rotateY(spin);
		ent.draw(app->camera);
		spin += 3.1415f / 180.0f;
		app->gui.drawTextureWindowGL("Duck Texture", ent.model.meshes[0].textures[0].getId());
		app->gui.drawTextureWindowGL("Cross Texture", crossTexture.getId());
	});

	ent.destroy();
	app.destroy();

	exit(EXIT_SUCCESS);
}
