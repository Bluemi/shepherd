#ifndef __BLOCK_CONTAINER_CLASS__
#define __BLOCK_CONTAINER_CLASS__

#include <vector>

#include "world_block.hpp"

class cuboid;

class block_container {
	public:
		block_container();
		block_container(const std::vector<world_block>& blocks);

		static std::vector<world_block> create_field(unsigned int seed);

		glm::vec3 get_respawn_position() const;

		void set_blocks(const std::vector<world_block>& blocks);
		const std::vector<world_block>& get_blocks() const;
		std::vector<world_block>& get_blocks();
		world_block& get_block(unsigned int x, unsigned int z);
		const world_block& get_block(unsigned int x, unsigned int z) const;

		// std::vector<world_block*> get_colliding_blocks(const cuboid&);
		std::vector<const world_block*> get_colliding_blocks(const cuboid&) const;
		float get_lower_y() const;
	private:
		std::vector<world_block> _blocks;
		float _lower_y;
};

#endif
