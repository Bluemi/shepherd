#include "controller.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

const Key controller::CAMERA_FORWARD_KEY = GLFW_KEY_W;
const Key controller::CAMERA_BACKWARD_KEY = GLFW_KEY_S;
const Key controller::CAMERA_LEFT_KEY = GLFW_KEY_A;
const Key controller::CAMERA_RIGHT_KEY = GLFW_KEY_D;
const Key controller::CAMERA_TOP_KEY = GLFW_KEY_SPACE;
const Key controller::CAMERA_BOTTOM_KEY = GLFW_KEY_LEFT_CONTROL;
const Key controller::HOOK_KEY = GLFW_KEY_E;
const Key controller::CLOSE_KEY = GLFW_KEY_ESCAPE;

controller::controller()
	: _mouse_x_change(0.0), _mouse_y_change(0.0), _left_mouse_pressed(false), _right_mouse_pressed(false)
{
	std::vector<Key> keys_to_track = {
		CAMERA_LEFT_KEY,
		CAMERA_RIGHT_KEY,
		CAMERA_FORWARD_KEY,
		CAMERA_BACKWARD_KEY,
		CAMERA_TOP_KEY,
		CAMERA_BOTTOM_KEY,
		HOOK_KEY,
		CLOSE_KEY
	};

	for (Key key : keys_to_track) {
		_is_pressed[key] = false;
	}
}

void controller::mouse_callback(double x, double y) {
	_mouse_x_change += x;
	_mouse_y_change += y;
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

	_left_mouse_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	_right_mouse_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

glm::vec2 controller::poll_mouse_changes() {
	glm::vec2 changes(_mouse_x_change, _mouse_y_change);
	_mouse_x_change = 0.f;
	_mouse_y_change = 0.f;
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
		std::cerr << "controller: asked for key: " << key << " but not found" << std::endl;
		return false;
	}
	return it->second;
}

bool controller::is_left_mouse_pressed() const {
	return _left_mouse_pressed;
}

bool controller::is_right_mouse_pressed() const {
	return _right_mouse_pressed;
}
