#pragma once
#include <glm/glm.hpp>

#include "Transform.h"

namespace Core {
	namespace Render {
		class Camera {
		public:
			Camera();
			~Camera();

			glm::mat4 projectionMatrix();
			glm::mat4 viewMatrix();
			Transform view;

			void setProjection(float fov, float aspect, float near, float far);

		private:
			glm::mat4 projection;
		};
	}
}