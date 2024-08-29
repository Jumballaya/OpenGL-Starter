#pragma once
#include <glm/glm.hpp>

#include "Transform.h"
#include "Core/GL/UniformBuffer.h"

namespace Core {
	namespace Render {

		struct CameraData {
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
			glm::vec4 camera_position;
		};

		class Camera {
		public:
			Camera();
			~Camera();

			void setProjection(float fov, float aspect, float near, float far);

			void moveTo(glm::vec3 position);
			
			void setup();
			void update();
			void destroy();

			glm::vec3 getPosition();

		private:
			Transform view;
			GL::UniformBuffer<CameraData> ubo;
			CameraData cameraData;
		};
	}
}