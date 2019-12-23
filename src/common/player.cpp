#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

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

void player::set_name(const std::string& name) {
	_name = name;
}

void player::set_position(const glm::vec3& position) {
	_position = position;
}

void player::set_speed(const glm::vec3& speed) {
	_speed = speed;
}
