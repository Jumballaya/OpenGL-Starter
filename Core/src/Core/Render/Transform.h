#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Core {
	namespace Render {
		class Transform {
		public:
			Transform();
			~Transform() = default;

			void rotateX(float x);
			void rotateY(float y);
			void rotateZ(float z);

			void translate(glm::vec3 t);
			void rotate(glm::vec3 r);
			void scale(glm::vec3 s);

			glm::mat4 matrix();

		private:
			glm::vec3 rotation;
			glm::vec3 _scale;
			glm::vec3 translation;

			glm::mat4 transformMatrix;

			void updateMatrix();
		};
	}
}
