#include "world_block.hpp"

#include <iostream>

#include "../physics/util.hpp"

world_block::world_block(const glm::vec3& position)
	: _position(position), _block_type(block_type::NORMAL)
{}

world_block::world_block(const glm::vec3& position, const glm::vec3& color)
	: _position(position), _color(color), _block_type(block_type::NORMAL)
{}

world_block::world_block(const glm::vec3& position, const glm::vec3& color, const block_type& block_type)
	: _position(position), _color(color), _block_type(block_type)
{}

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

bool world_block::is_winning_block() const {
	return _block_type == block_type::WINNING;
}
