#include "Transform.h"

namespace Core {
	namespace Render {
		Transform::Transform() {
			rotation = glm::vec3(0.0f);
			_scale = glm::vec3(1.0f);
			translation = glm::vec3(0.0f);
			transformMatrix = glm::identity<glm::mat4>();
		}

		void Transform::rotateX(float x) {
			rotation[0] = x;
			updateMatrix();
		}

		void Transform::rotateY(float y) {
			rotation[1] = y;
			updateMatrix();
		}

		void Transform::rotateZ(float z) {
			rotation[2] = z;
			updateMatrix();
		}

		void Transform::translate(glm::vec3 t) {
			translation[0] = t[0];
			translation[1] = t[1];
			translation[2] = t[2];
			updateMatrix();
		}

		void Transform::rotate(glm::vec3 r) {
			rotation[0] = r[0];
			rotation[1] = r[1];
			rotation[2] = r[2];
			updateMatrix();
		}

		void Transform::scale(glm::vec3 s) {
			_scale[0] = s[0];
			_scale[1] = s[1];
			_scale[2] = s[2];
			updateMatrix();
		}


		glm::mat4 Transform::matrix() {
			return transformMatrix;
		}

		void Transform::updateMatrix() {
			transformMatrix = glm::identity<glm::mat4>();
			transformMatrix = glm::translate(transformMatrix, translation);
			transformMatrix = glm::rotate(transformMatrix, rotation[0], glm::vec3(1.0, 0.0, 0.0)); // rotX
			transformMatrix = glm::rotate(transformMatrix, rotation[1], glm::vec3(0.0, 1.0, 0.0)); // rotY
			transformMatrix = glm::rotate(transformMatrix, rotation[2], glm::vec3(0.0, 0.0, 1.0)); // rotZ
			transformMatrix = glm::scale(transformMatrix, _scale);
		}
	}
}
