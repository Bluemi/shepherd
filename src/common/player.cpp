#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "networking/actions_packet.hpp"

const float PLAYER_ROTATE_SPEED = 0.07f;

player::player(unsigned int id, const std::string& name)
	: _id(id), _name(name)
{}

player::player(unsigned int id, const std::string& name, const glm::vec3& position)
	: _id(id), _name(name), _position(position)
{}

unsigned int player::get_id() const {
	return _id;
}

const std::string& player::get_name() const {
	return _name;
}

const glm::vec3& player::get_position() const {
	return _position;
}

const glm::vec3& player::get_speed() const {
	return _speed;
}

std::uint8_t player::get_actions() const {
	return _actions;
}

void player::set_name(const std::string& name) {
	_name = name;
}

void player::set_position(const glm::vec3& position) {
	_position = position;
}

void player::set_speed(const glm::vec3& speed) {
	_speed = speed;
}

void player::set_actions(const std::uint8_t actions) {
	_actions = actions;
}

void player::update_direction(const glm::vec2& direction_update) {
	_yaw += direction_update.x * PLAYER_ROTATE_SPEED;
	_pitch -= direction_update.y * PLAYER_ROTATE_SPEED;
	_pitch = fmax(fmin(_pitch, 89.f), -89.0f);
}

glm::vec3 player::get_up() {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 player::get_right() const {
	return glm::normalize(glm::cross(get_direction(), player::get_up()));
}

glm::vec3 player::get_direction() const {
	return glm::normalize(glm::vec3(
				cos(glm::radians(_pitch)) * cos(glm::radians(_yaw)),
				sin(glm::radians(_pitch)),
				cos(glm::radians(_pitch)) * sin(glm::radians(_yaw))));
}

glm::vec3 player::get_top() const {
	return glm::normalize(glm::cross(get_right(), get_direction()));
}

glm::mat4 player::get_look_at() const {
	return glm::lookAt(_position, _position + get_direction(), get_up());
}

void player::tick() {
	float forward = 0;
	if (_actions & FORWARD_ACTION)
		forward++;
	if (_actions & BACKWARD_ACTION)
		forward--;

	float right = 0;
	if (_actions & LEFT_ACTION)
		right--;
	if (_actions & RIGHT_ACTION)
		right++;

	_speed += (get_right()*right + get_direction()*forward)*0.01f;

	_speed *= 0.98;

	_position += _speed;
}
