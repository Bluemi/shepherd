#include "world_block.hpp"

#include <iostream>

#include "../physics/util.hpp"

world_block::world_block(const glm::ivec3& position)
	: _position(position), _block_type(block_type::NORMAL)
{}

world_block::world_block(const glm::ivec3& position, const block_type block_type)
	: _position(position), _block_type(block_type)
{}

const glm::ivec3& world_block::get_position() const {
	return _position;
}

block_type world_block::get_type() const {
	return _block_type;
}

void world_block::set_position(const glm::ivec3& position) {
	_position = position;
}

bool world_block::is_winning_block() const {
	return _block_type == block_type::WINNING;
}
