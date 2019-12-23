#include "controller.hpp"

#include <GLFW/glfw3.h>

#include "../camera/camera.hpp"

const Key controller::CAMERA_FORWARD_KEY = GLFW_KEY_W;
const Key controller::CAMERA_BACKWARD_KEY = GLFW_KEY_S;
const Key controller::CAMERA_LEFT_KEY = GLFW_KEY_A;
const Key controller::CAMERA_RIGHT_KEY = GLFW_KEY_D;
const Key controller::CAMERA_TOP_KEY = GLFW_KEY_SPACE;
const Key controller::CAMERA_BOTTOM_KEY = GLFW_KEY_LEFT_CONTROL;
const Key controller::CLOSE_KEY = GLFW_KEY_ESCAPE;

controller::controller() {
	std::vector<Key> keys_to_track = {
		CAMERA_LEFT_KEY,
		CAMERA_RIGHT_KEY,
		CAMERA_FORWARD_KEY,
		CAMERA_BACKWARD_KEY,
		CLOSE_KEY
	};

	for (Key key : keys_to_track) {
		_is_pressed[key] = false;
	}
}

void controller::mouse_callback(double x, double y) {
	x_change += x;
	y_change += y;
}

void controller::process_user_input(GLFWwindow* window) {
	for (auto it = _is_pressed.begin(); it != _is_pressed.end(); ++it) {
		const bool key_pressed = glfwGetKey(window, it->first) == GLFW_PRESS;
		if (key_pressed != it->second) {
			it->second = key_pressed;
			if (key_pressed) {
				this->key_pressed(window, it->first);
			} else {
				this->key_released(window, it->first);
			}
		}
	}
}

glm::vec2 controller::poll_mouse_changes() {
	glm::vec2 changes(x_change, y_change);
	x_change = 0.f;
	y_change = 0.f;
	return changes;
}

void controller::key_pressed(GLFWwindow* window, Key key) {
	if (key == CLOSE_KEY)
		glfwSetWindowShouldClose(window, true);
}

void controller::key_released(GLFWwindow*, Key) {
}

bool controller::is_key_pressed(const Key key) const {
	auto it = _is_pressed.find(key);
	if (it == _is_pressed.end()) {
		return false;
	}
	return it->second;
}
