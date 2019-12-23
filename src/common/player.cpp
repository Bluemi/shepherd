#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "networking/actions_packet.hpp"

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

void player::tick() {
	int forward = 0;
	if (_actions & FORWARD_ACTION)
		forward++;
	if (_actions & BACKWARD_ACTION)
		forward--;

	int left = 0;
	if (_actions & LEFT_ACTION)
		left++;
	if (_actions & RIGHT_ACTION)
		left--;

	_speed.x -= left*0.01f;
	_speed.z += forward*0.01f;

	_speed *= 0.98;

	_position += _speed;
}
