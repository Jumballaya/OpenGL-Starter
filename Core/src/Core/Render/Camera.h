#pragma once
#include <glm/glm.hpp>

#include "Transform.h"
#include "Core/GL/UniformBuffer.h"

namespace Core {
	namespace Render {

		struct CameraData {
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
		};

		class Camera {
		public:
			Camera();
			~Camera();

			void setProjection(float fov, float aspect, float near, float far);
			
			void setup();
			void update();
			void destroy();

		private:
			Transform view;
			GL::UniformBuffer<CameraData> ubo;
			CameraData cameraData;
		};
	}
}