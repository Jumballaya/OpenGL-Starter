#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace Core {
	namespace Render {	
		Camera::Camera() {
			cameraData.projection_matrix = glm::perspective(glm::radians(70.0f), 4.0f / 3.0f, 0.01f, 100.0f);
			cameraData.view_matrix = view.matrix();
			cameraData.camera_position = glm::vec4(0.0f);
		}

		Camera::~Camera() {
			destroy();
		}

		void Camera::setup() {
			ubo.setup();
		}

		void Camera::destroy() {
			ubo.destroy();
		}

		void Camera::setProjection(float fov, float aspect, float near, float far) {
			cameraData.projection_matrix = glm::perspective(fov, aspect, near, far);
		}

		void Camera::update() {
			ubo.setData(cameraData);
			ubo.bind(0);
		}

		void Camera::moveTo(glm::vec3 position) {
			cameraData.camera_position.x = position.x;
			cameraData.camera_position.y = position.y;
			cameraData.camera_position.z = position.z;
		}

	}
}
