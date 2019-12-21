#include "controller.hpp"

#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
	std::cout << "controller::mouse_callback()" << std::endl;
	x_change += x;
	y_change += y;
}

void controller::process_user_input(GLFWwindow* window, camera* camera) {
	std::cout << "controller::process_user_input()" << std::endl;
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

	process_camera(camera);
}

void controller::process_camera(camera* camera) {
	glm::vec3 acceleration;
	if (_is_pressed[CAMERA_FORWARD_KEY])
		acceleration.x += 1.f;
	if (_is_pressed[CAMERA_BACKWARD_KEY])
		acceleration.x -= 1.f;
	if (_is_pressed[CAMERA_RIGHT_KEY])
		acceleration.z += 1.f;
	if (_is_pressed[CAMERA_LEFT_KEY])
		acceleration.z -= 1.f;
	if (_is_pressed[CAMERA_TOP_KEY])
		acceleration.y += 1.f;
	if (_is_pressed[CAMERA_BOTTOM_KEY])
		acceleration.y -= 1.f;

	if (camera != nullptr) {
		camera->set_acceleration(acceleration);
		camera->change_direction(glm::vec2(x_change, y_change));
		x_change = 0.0;
		y_change = 0.0;
	}
}

void controller::key_pressed(GLFWwindow* window, Key key) {
	if (key == CLOSE_KEY)
		glfwSetWindowShouldClose(window, true);
}

void controller::key_released(GLFWwindow*, Key) {
}
