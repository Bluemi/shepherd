#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

player::player(unsigned int id, const std::string& name): _id(id), _name(name) {}

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
