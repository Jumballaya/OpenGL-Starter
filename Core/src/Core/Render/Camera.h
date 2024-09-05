#pragma once
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Transform.h"
#include "Core/GL/UniformBuffer.h"
#include "Core/IO/Inputs.h"

namespace Core {
	namespace Render {

		class CameraController {
		public:
			virtual ~CameraController() = default;
			virtual glm::mat4 getViewMatrix() const = 0;
			virtual glm::vec3 getPosition() const = 0;
			virtual void moveTo(glm::vec3 pos) = 0;
			virtual void update(double dT, IO::Inputs& inputs) = 0;
		};

		class CameraControllerFPS final : public CameraController {
		public:
			CameraControllerFPS() {
				position = glm::vec3(0.0f);
				orientation = glm::lookAt(position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			};
			CameraControllerFPS(
				const glm::vec3& pos,
				const glm::vec3& target,
				const glm::vec3& up) : position(pos), orientation(glm::lookAt(pos, target, up)) {}

			virtual void update(double dT, IO::Inputs& inputs) override {
				glm::vec2 _mousePos = inputs.getMousePosition();
				if (inputs.isMousePressed(IO::MouseButton_Left)) {
					const glm::vec2 delta = _mousePos - mousePos;
					const glm::quat deltaQuat = glm::quat(glm::vec3(
						mouseSpeed * delta.y,
						mouseSpeed * delta.x,
						0.0f));
					orientation = glm::normalize(deltaQuat * orientation);
				}
				mousePos = _mousePos;
				const glm::mat4 v = glm::mat4_cast(orientation);
				const glm::vec3 forward = -glm::vec3(v[0][2], v[1][2], v[2][2]);
				const glm::vec3 right = glm::vec3(v[0][0], v[1][0], v[2][0]);
				const glm::vec3 up = glm::cross(right, forward);
				glm::vec3 accel(0.0f);
				if (inputs.isKeyPressed(87)) accel += forward;
				if (inputs.isKeyPressed(83)) accel -= forward;
				if (inputs.isKeyPressed(65)) accel -= right;
				if (inputs.isKeyPressed(68)) accel += right;
				if (inputs.isKeyPressed(32)) accel += up;
				if (inputs.isKeyPressed(67)) accel -= up;
				if (inputs.isKeyPressed(340)) accel *= fastCoef;

				if (accel == glm::vec3(0)) {
					moveSpeed -= moveSpeed * std::min((1.0f / damping) * static_cast<float>(dT), 1.0f);
				} else {
					moveSpeed += accel * acceleration * static_cast<float>(dT);
					const float max = inputs.isKeyPressed(340) ? maxSpeed * fastCoef : maxSpeed;
					if (glm::length(moveSpeed) > max) {
						moveSpeed = glm::normalize(moveSpeed) * max;
					}
				}
				position += moveSpeed * static_cast<float>(dT);
			}

			virtual glm::mat4 getViewMatrix() const override {
				const glm::mat4 t = glm::translate(glm::mat4(1.0f), -position);
				const glm::mat4 r = glm::mat4_cast(orientation);
				return r * t;
			}

			virtual glm::vec3 getPosition() const override {
				return position;
			}

			virtual void moveTo(glm::vec3 pos) override {
				position = pos;
			}

			void setUpVector(const glm::vec3& up) {
				const glm::mat4 view = getViewMatrix();
				const glm::vec3 dir = -glm::vec3(view[0][2], view[1][2], view[2][2]);
				orientation = glm::lookAt(position, position + dir, up);
			}

			void resetMousePosition(const glm::vec2& pos) {
				mousePos = pos;
			}

			float mouseSpeed = 4.0f;
			float acceleration = 150.0f;
			float damping = 0.2f;
			float maxSpeed = 10.0f;
			float fastCoef = 10.0f;
		private:
			glm::vec2 mousePos = glm::vec2(0);
			glm::vec3 position = glm::vec3(0.0f, 10.0f, 10.0f);
			glm::quat orientation = glm::quat(glm::vec3(0));
			glm::vec3 moveSpeed = glm::vec3(0.0f);
		};


		struct CameraData {
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
			glm::vec4 camera_position;
		};

		class Camera final {
		public:
			Camera() {
				projectionMatrix = glm::perspective(45.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
			};

			void initialize();
			void update(double dT, IO::Inputs& inputs);

			void setProjection(float fov, float aspect, float near, float far);
			void moveTo(glm::vec3 position);
			void updateData();

			glm::vec3 getPosition() const { return controller.getPosition(); };
			glm::mat4 getViewMatrix() const { return controller.getViewMatrix(); };
			glm::mat4 getProjectionMatrix() const { return projectionMatrix; }

			static Camera CreateFPSCamera() {
				return Camera();
			};

			GL::UniformBuffer<CameraData> ubo;
			CameraControllerFPS controller;
		private:
			glm::mat4 projectionMatrix;
		};
	}
}