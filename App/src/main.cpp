#include "App.h"

//#include <glm/glm.hpp>
//#include <Core/Math/math.h>

//glm::vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize) {
//	const float a = 2.0f * float(i) / faceSize;
//	const float b = 2.0f * float(j) / faceSize;
//	switch (faceID) {
//	case 0:
//		return glm::vec3(-1.0f, a - 1.0f, b - 1.0f);
//	case 1:
//		return glm::vec3(a - 1.0f, -1.0f, 1.0f - b);
//	case 2:
//		return glm::vec3(1.0f, a - 1.0f, 1.0f - b);
//	case 3:
//		return glm::vec3(1.0f, 1.0f, 1.0f - b);
//	case 4:
//		return glm::vec3(b - 1.0f, a - 1.0f, 1.0f);
//	case 5:
//		return glm::vec3(1.0f - b, a - 1.0f, -1.0f);
//	}
//	return glm::vec3(0.0f);
//}
//
//Core::Render::Bitmap convertEquirectangularMapToVerticalCross(Core::Render::Bitmap& b) {
//	if (b.getType() != Core::Render::BitmapType_2D) return Core::Render::Bitmap();
//	const int faceSize = b.getWidth();
//	const int w = faceSize * 3;
//	const int h = faceSize * 4;
//	Core::Render::Bitmap result;
//	result.setup();
//	result.load(w, h, b.getComponentCount(), b.getFormat());
//
//	const glm::ivec2 kFaceOffsets[] = {
//		glm::ivec2(faceSize, faceSize * 3),
//		glm::ivec2(0, faceSize),
//		glm::ivec2(faceSize, faceSize),
//		glm::ivec2(faceSize * 2, faceSize),
//		glm::ivec2(faceSize, 0),
//		glm::ivec2(faceSize, faceSize * 2)
//	};
//
//	const int clampW = b.getWidth() - 1;
//	const int clampH = b.getHeight() - 1;
//
//	for (int face = 0; face < 6; face++) {
//		for (int i = 0; i < faceSize; i++) {
//			for (int j = 0; j < faceSize; j++) {
//				const glm::vec3 P = faceCoordsToXYZ(i, j, face, faceSize);
//				const float R = hypot(P.x, P.y);
//				const float theta = atan2(P.y, P.x);
//				const float phi = atan2(P.z, R);
//				const float Uf = float(2.0f * faceSize * (theta + Core::Math::PI) / Core::Math::PI);
//				const float Vf = float(2.0f * faceSize * (Core::Math::PI / 2.0f - phi) / Core::Math::PI);
//
//				// 4 samples for bilinear interpolation
//				const int U1 = Core::Math::clamp(int(floor(Uf)), 0, clampW);
//				const int V1 = Core::Math::clamp(int(floor(Vf)), 0, clampH);
//				const int U2 = Core::Math::clamp(U1 + 1, 0, clampW);
//				const int V2 = Core::Math::clamp(V1 + 1, 0, clampH);
//				const float s = Uf - U1;
//				const float t = Vf - V1;
//
//				// fetch 4 samples
//				const glm::vec4 A = b.getPixel(U1, V1);
//				const glm::vec4 B = b.getPixel(U2, V1);
//				const glm::vec4 C = b.getPixel(U1, V2);
//				const glm::vec4 D = b.getPixel(U2, V2);
//
//				// bilinear interpolation
//				const glm::vec4 color = A * (1 - s) * (1 - t) + B * (s) * (1 - t) + C * (1 - s) * t + D * (s) * (t);
//				result.setPixel(i + kFaceOffsets[face].x, j + kFaceOffsets[face].y, color);
//			}
//		}
//
//		return result;
//	}
//}
//
//Core::Render::Bitmap convertVerticalCrossToCubeMapFaces(Core::Render::Bitmap& b) {
//	const int faceWidth = b.getWidth() / 3;
//	const int faceHeight = b.getHeight() / 4;
//	Core::Render::Bitmap cubemap;
//	cubemap.setup();
//	cubemap.load(faceWidth, faceHeight, 6, b.getComponentCount(), b.getFormat());
//}

int main() {
	App app;
	Entity ent;
	ent.setup();
	ent.loadShader("shaders/model/vertex.glsl", "shaders/model/fragment.glsl");
	ent.loadModel("models/duck/Duck.gltf");

	ent.transform.scale(glm::vec3(0.05f));
	ent.transform.translate(glm::vec3(0.0f, -4.0f, -12.0f));

	glClearColor(0.4f, 0.35f, 0.4f, 1.0f);

	float spin = 0.0f;
	app.run([&ent, &spin](App* app, int width, int height) {
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		ent.transform.rotateY(spin);
		ent.draw(app->camera);
		spin += 3.1415f / 180.0f;
	});

	ent.destroy();
	app.destroy();

	exit(EXIT_SUCCESS);
}
