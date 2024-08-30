#include "Inputs.h"

namespace Core {
	namespace IO {
	
		void Inputs::setup() {}

		void Inputs::load(std::vector<unsigned int> keys) {
			for (auto key : keys) {
				keystate[key] = false;
			}
			mousestate[MouseButton_Left] = false;
			mousestate[MouseButton_Right] = false;
			mousestate[MouseButton_Middle] = false;

			mousePosition = glm::vec2(0.0f);
		}

		void Inputs::updateKeys(unsigned int key, bool pressed) {
			keystate[key] = pressed;
		}

		void Inputs::updateMousePosition(glm::vec2& pos) {
			mousePosition.x = pos.x;
			mousePosition.y = pos.y;
		}

		void Inputs::updateMouseButton(MouseButton button, bool pressed) {
			mousestate[button] = pressed;
		}

		bool Inputs::isMousePressed(MouseButton button) {
			if (mousestate.find(button) == mousestate.end()) {
				return false;
			}
			return mousestate[button];
		}

		bool Inputs::isKeyPressed(unsigned int key) {
			if (keystate.find(key) == keystate.end()) {
				return false;
			}
			return keystate[key];
		}

		glm::vec2 Inputs::getMousePosition() const {
			return mousePosition;
		}
	}
}