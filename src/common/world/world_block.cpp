#include "world_block.hpp"

#include "perlin_noise.hpp"

constexpr unsigned int MAP_SIZE = 50;

world_block::world_block(const glm::vec3& position) : _position(position) {}

std::vector<world_block> world_block::create_field(unsigned int seed) {
	std::vector<world_block> blocks;
	perlin_noise pn;

	unsigned int s = seed % 25000;

	// initialize blocks
	for (unsigned int x = 0; x <= MAP_SIZE; x++) {
		for (unsigned int z = 0; z <= MAP_SIZE; z++) {
			float y = pn({
				(x+s)*0.1f,
				(z+s)*0.1f
			});

			y = glm::floor(y*2.f);
			blocks.push_back(world_block(glm::vec3(x, y, z)));
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

block_container::block_container() {}
block_container::block_container(const std::vector<world_block>& blocks) : _blocks(blocks) {}

const std::vector<world_block>& block_container::get_blocks() const {
	return _blocks;
}

std::vector<world_block>& block_container::get_blocks() {
	return _blocks;
}

world_block& block_container::get_block(unsigned int x, unsigned int z) {
	return _blocks[x*MAP_SIZE + z];
}

const world_block& block_container::get_block(unsigned int x, unsigned int z) const {
	return _blocks[x*MAP_SIZE + z];
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

	unsigned int min_x_index_clamped = glm::clamp(min_x_index, 0, static_cast<int>(MAP_SIZE));
	unsigned int max_x_index_clamped = glm::clamp(max_x_index, 0, static_cast<int>(MAP_SIZE));
	unsigned int min_z_index_clamped = glm::clamp(min_z_index, 0, static_cast<int>(MAP_SIZE));
	unsigned int max_z_index_clamped = glm::clamp(max_z_index, 0, static_cast<int>(MAP_SIZE));

	for (unsigned int x = min_x_index_clamped; x <= max_x_index_clamped; x++) {
		for (unsigned int z = min_z_index_clamped; z <= max_z_index_clamped; z++) {
			const world_block& b = get_block(x, z);
			if (
				b.get_position().y - 0.5f < box.get_max_y() &&
				b.get_position().y + 0.5f > box.get_min_y()
			) {
				colliding_blocks.push_back(&b);
			}
		}
	}

	return colliding_blocks;
}
