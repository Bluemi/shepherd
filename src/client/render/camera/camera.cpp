#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

const float camera::CAMERA_SPEED = 0.07f;
const float camera::CAMERA_ROTATE_SPEED = 0.1f;
const float camera::CAMERA_DRAG = 0.78f;

camera::camera() {}

camera::camera(const glm::vec3 &position, const float pitch, const float yaw)
	: _position(position), _pitch(pitch), _yaw(yaw)
{}

void camera::tick(const double speed) {
	_speed += _acceleration * CAMERA_SPEED;
	_speed *= CAMERA_DRAG;
	_position += _speed * static_cast<float>(speed);
}

void camera::stop() {
	_acceleration = glm::vec3();
}

glm::vec3 camera::get_up() {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 camera::get_right() const {
	return glm::normalize(glm::cross(get_direction(), camera::get_up()));
}

glm::vec3 camera::get_direction() const {
	return glm::normalize(glm::vec3(
				cos(glm::radians(_pitch)) * cos(glm::radians(_yaw)),
				sin(glm::radians(_pitch)),
				cos(glm::radians(_pitch)) * sin(glm::radians(_yaw))));
}

glm::vec3 camera::get_top() const {
	return glm::normalize(glm::cross(get_right(), get_direction()));
}

glm::mat4 camera::get_look_at() const {
	return glm::lookAt(_position, _position + get_direction(), get_up());
}

glm::vec3 camera::get_position() const {
	return _position;
}

void camera::change_direction(glm::vec2 value) {
	_yaw += value.x * CAMERA_ROTATE_SPEED;
	_pitch -= value.y * CAMERA_ROTATE_SPEED;
	_pitch = fmax(fmin(_pitch, 89.f), -89.0f);
}

void camera::set_acceleration(const glm::vec3& acceleration) {
	glm::vec3 a;
	a = get_direction() * acceleration.x;
	a += get_top() * acceleration.y;
	a += get_right() * acceleration.z;
	_acceleration = a;
}
