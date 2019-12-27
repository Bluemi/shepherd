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

const glm::vec2& player::get_view_angles() const {
	return _view_angles;
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

void player::set_view_angles(const glm::vec2& view_angles) {
	_view_angles = view_angles;
}

void player::set_speed(const glm::vec3& speed) {
	_speed = speed;
}

void player::set_actions(const std::uint8_t actions) {
	_actions = actions;
}

void player::update_direction(const glm::vec2& direction_update) {
	_view_angles.y += direction_update.x * PLAYER_ROTATE_SPEED;
	_view_angles.x -= direction_update.y * PLAYER_ROTATE_SPEED;
	_view_angles.x = fmax(fmin(_view_angles.x, 89.f), -89.0f);
}

glm::vec3 player::get_up() {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 player::get_right() const {
	return glm::normalize(glm::cross(get_direction(), player::get_up()));
}

glm::vec3 player::get_direction() const {
	return glm::normalize(glm::vec3(
				cos(glm::radians(_view_angles.x)) * cos(glm::radians(_view_angles.y)),
				sin(glm::radians(_view_angles.x)),
				cos(glm::radians(_view_angles.x)) * sin(glm::radians(_view_angles.y))
			));
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

	float up = 0;
	if (_actions & JUMP_ACTION)
		up++;
	if (_actions & BOTTOM_ACTION)
		up--;

	_speed += (get_right()*right + get_direction()*forward + get_top()*up)*0.1f;

	_speed *= 0.78;

	_position += _speed;
}
