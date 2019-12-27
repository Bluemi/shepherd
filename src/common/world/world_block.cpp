#include "world_block.hpp"

world_block::world_block(const glm::vec3& position) : _position(position) {}

const glm::vec3& world_block::get_position() const {
	return _position;
}

const glm::vec3& world_block::get_color() const {
	return _color;
}

void world_block::set_position(const glm::vec3& position) {
	_position = position;
}

void world_block::set_color(const glm::vec3& color) {
	_color = color;
}
