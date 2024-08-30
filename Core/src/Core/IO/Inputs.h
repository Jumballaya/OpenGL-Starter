#pragma once

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Core {
	namespace IO {
		struct MouseState {
		public:
			glm::vec2 pos = glm::vec2(0.0f);
			bool left = false;
			bool right = false;
		};

		enum MouseButton {
			MouseButton_Left,
			MouseButton_Right,
			MouseButton_Middle,
		};

		class Inputs {
		public:
			Inputs() = default;
			~Inputs() = default;

			void setup();
			void load(std::vector<unsigned int>keys);
			void updateKeys(unsigned int key, bool pressed);
			void updateMousePosition(glm::vec2& pos);
			void updateMouseButton(MouseButton button, bool pressed);

			bool isMousePressed(MouseButton button);
			bool isKeyPressed(unsigned int key);
			glm::vec2& getMousePosition();

		private:
			std::unordered_map<unsigned int, bool> keystate;
			std::unordered_map<MouseButton, bool> mousestate;
			glm::vec2 mousePosition;

		};
	}
}
