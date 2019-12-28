#include "world_block.hpp"

#include <iostream>
#include <glm/gtc/noise.hpp>

#include "../physics/util.hpp"

constexpr unsigned int MAP_X_SIZE = 64;
constexpr unsigned int MAP_Z_SIZE = 16;
constexpr float NOISE_SCALE = 0.05f;
constexpr float MAP_HEIGHT = 10.f;
constexpr float WINNING_COLOR_WHITE = 0.3f;
constexpr float WINNING_COLOR_BLACK = 0.03f;

world_block::world_block(const glm::vec3& position)
	: _position(position), _block_type(block_type::NORMAL)
{}

world_block::world_block(const glm::vec3& position, const glm::vec3& color)
	: _position(position), _color(color), _block_type(block_type::NORMAL)
{}

world_block::world_block(const glm::vec3& position, const glm::vec3& color, const block_type& block_type)
	: _position(position), _color(color), _block_type(block_type)
{}

std::vector<world_block> world_block::create_field(unsigned int seed) {
	std::vector<world_block> blocks;

	unsigned int s = seed % 25000;

	// initialize blocks
	for (unsigned int x = 0; x < MAP_X_SIZE; x++) {
		for (unsigned int z = 0; z < MAP_Z_SIZE; z++) {
			float y = glm::perlin(glm::vec2((x+s)*NOISE_SCALE, (z+s)*NOISE_SCALE));
			y = glm::floor(y*MAP_HEIGHT / (1.f + glm::exp(static_cast<float>(x)-MAP_X_SIZE+6.f)));

			// colors
			float red   = (x+z)%2?WINNING_COLOR_WHITE:WINNING_COLOR_BLACK;
			float green = (x+z)%2?WINNING_COLOR_WHITE:WINNING_COLOR_BLACK;
			float blue  = (x+z)%2?WINNING_COLOR_WHITE:WINNING_COLOR_BLACK;

			// block type
			block_type bt = block_type::NORMAL;
			if (x > MAP_X_SIZE-3) {
				bt = block_type::WINNING;
				y = 0.f;
			} else {
				blue  = glm::perlin(glm::vec2(x*0.1f        , z*0.1f + 100.f ))*0.02f + 0.03f;
				red   = glm::perlin(glm::vec2(x*0.1f + 200.f, z*0.1f + 300.f ))*0.02f + 0.1f - glm::max(blue, 0.f)*0.6f;
				green = glm::perlin(glm::vec2(x*0.1f + 400.f, z*0.1f + 500.f ))*0.03f + 0.12f - glm::max(blue, 0.f)*0.3f;
			}

			blocks.push_back(world_block(glm::vec3(x, y, z), glm::vec3(red, green, blue), bt));
		}
	}

	return blocks;
}

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

block_container::block_container() {}
block_container::block_container(const std::vector<world_block>& blocks) : _blocks(blocks) {}

glm::vec3 block_container::get_respawn_position() const {
	unsigned int x = (rand() % 5)+1;
	unsigned int z = rand() % MAP_Z_SIZE;

	glm::vec3 spawn_pos = get_block(x, z).get_position();
	spawn_pos.y += 2.f;
	return spawn_pos;
}

const std::vector<world_block>& block_container::get_blocks() const {
	return _blocks;
}

std::vector<world_block>& block_container::get_blocks() {
	return _blocks;
}

world_block& block_container::get_block(unsigned int x, unsigned int z) {
	return _blocks[x*MAP_Z_SIZE + z];
}

const world_block& block_container::get_block(unsigned int x, unsigned int z) const {
	return _blocks[x*MAP_Z_SIZE + z];
}

/*
std::vector<world_block*> block_container::get_colliding_blocks(const cuboid& box) {
	return const_cast<std::vector<world_block*>>(const_cast<const block_container*>(this)->get_colliding_blocks(box));
}
*/

std::vector<const world_block*> block_container::get_colliding_blocks(const cuboid& box) const {
	std::vector<const world_block*> colliding_blocks;

	int min_x_index = static_cast<int>(glm::floor(box.get_min_x() + 0.5f));
	int max_x_index = static_cast<int>(glm::floor(box.get_max_x() + 0.5f));
	int min_z_index = static_cast<int>(glm::floor(box.get_min_z() + 0.5f));
	int max_z_index = static_cast<int>(glm::floor(box.get_max_z() + 0.5f));

	unsigned int min_x_index_clamped = glm::clamp(min_x_index, 0, static_cast<int>(MAP_X_SIZE));
	unsigned int max_x_index_clamped = glm::clamp(max_x_index, 0, static_cast<int>(MAP_X_SIZE));
	unsigned int min_z_index_clamped = glm::clamp(min_z_index, 0, static_cast<int>(MAP_Z_SIZE));
	unsigned int max_z_index_clamped = glm::clamp(max_z_index, 0, static_cast<int>(MAP_Z_SIZE));

	for (unsigned int x = min_x_index_clamped; x <= max_x_index_clamped; x++) {
		for (unsigned int z = min_z_index_clamped; z <= max_z_index_clamped; z++) {
			const world_block& b = get_block(x, z);
			if (
				b.get_position().x - 0.5f < box.get_max_x() &&
				b.get_position().x + 0.5f > box.get_min_x() &&
				b.get_position().y - 0.5f < box.get_max_y() &&
				b.get_position().y + 0.5f > box.get_min_y() &&
				b.get_position().z - 0.5f < box.get_max_z() &&
				b.get_position().z + 0.5f > box.get_min_z()
			) {
				colliding_blocks.push_back(&b);
			}
		}
	}

	return colliding_blocks;
}
