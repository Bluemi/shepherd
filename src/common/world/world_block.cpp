#include "world_block.hpp"

#include "perlin_noise.hpp"

world_block::world_block(const glm::vec3& position) : _position(position) {}

std::vector<world_block> world_block::create_field(unsigned int seed) {
	std::vector<world_block> blocks;
	perlin_noise pn;

	unsigned int s = seed % 25000;

	// initialize blocks
	for (unsigned int x = 0; x <= 40; x++) {
		for (unsigned int z = 0; z <= 40; z++) {
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
