#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace Core {
	namespace Render {	
		Camera::Camera() {
			projection = glm::perspective(glm::radians(70.0f), 4.0f / 3.0f, 0.01f, 100.0f);
		}

		Camera::~Camera() {

		}

		void Camera::setProjection(float fov, float aspect, float near, float far) {
			projection = glm::perspective(fov, aspect, near, far);
		}

		glm::mat4 Camera::projectionMatrix() {
			return projection;
		}

		glm::mat4 Camera::viewMatrix() {
			return view.matrix();
		}
	}
}
