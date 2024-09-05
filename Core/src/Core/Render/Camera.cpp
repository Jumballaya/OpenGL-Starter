#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace Core {
	namespace Render {	
		void Camera::update(double dT, IO::Inputs& inputs) {
			controller.update(dT, inputs);
			updateData();
		}

		void Camera::initialize() {
			ubo.initialize();
		}

		void Camera::setProjection(float fov, float aspect, float near, float far) {
			projectionMatrix = glm::perspective(fov, aspect, near, far);
			updateData();
		}

		void Camera::moveTo(glm::vec3 position) {
			controller.moveTo(position);
			updateData();
		}

		void Camera::updateData() {
			glm::vec3 pos = controller.getPosition();
			ubo.update({
				.view_matrix = controller.getViewMatrix(),
				.projection_matrix = projectionMatrix,
				.camera_position = glm::vec4(pos[0], pos[1], pos[2], 1.0)
			});
		}

	}
}
