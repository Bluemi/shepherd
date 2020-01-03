#ifndef __BLOCK_CONTAINER_CLASS__
#define __BLOCK_CONTAINER_CLASS__

#include <vector>
#include <optional>
#include <memory>

#include "world_block.hpp"

class cuboid;
class ray;

constexpr unsigned int BLOCK_CHUNK_SIZE = 16;

class block_chunk {
	public:
		block_chunk();
		block_chunk(const glm::ivec3& origin);

		block_type get_block_type(const glm::ivec3& position) const;
		block_type get_local_block_type(const glm::uvec3& position) const;
		const std::vector<block_type>& get_block_types() const;
		const glm::ivec3& get_origin() const;
		glm::ivec3 get_top() const;
		bool contains(const glm::ivec3& position) const;

		void set_block_type(const glm::ivec3& position, block_type bt);
	private:
		static unsigned int get_index(const glm::uvec3& position);
		glm::uvec3 global_to_local(const glm::ivec3& position) const;

		// contains the blocks of the chunk
		// the first BLOCK_CHUNK_SIZE blocks have x/y coordinates=0
		std::vector<block_type> _block_types;

		// The position of the block with the lowest x/y/z coordinates in this chunk
		glm::ivec3 _origin;
};

class block_container {
	public:
		block_container();
		block_container(const std::vector<world_block>& blocks);

		static std::vector<world_block> create_field(unsigned int seed);
		static glm::ivec3 to_chunk_position(const glm::ivec3& position);
		static glm::vec3 get_color(const glm::ivec3& position);
		static glm::vec3 get_winning_color(const glm::ivec3& position);

		glm::vec3 get_respawn_position() const;

		std::optional<world_block> get_block(const glm::ivec3& position) const;
		const std::vector<std::shared_ptr<block_chunk>>& get_chunks() const;
		const block_chunk* get_containing_chunk(const glm::ivec3& position) const;
		block_chunk* get_containing_chunk(const glm::ivec3& position);
		std::vector<world_block> get_colliding_blocks(const cuboid&) const;
		std::optional<world_block> get_colliding_block(const ray& r, float max_range) const;
		std::optional<glm::vec3> get_collision_point(const ray& r, float max_range) const;
		std::optional<glm::ivec3> get_addition_position(const ray& r, float max_range) const;

		int get_min_y() const;

		void add_block(const glm::ivec3& position, block_type bt);
		block_chunk* add_chunk(const glm::ivec3& position);
		bool remove_block(const glm::ivec3& position);

	private:
		std::vector<std::shared_ptr<block_chunk>> _block_chunks;
		int _min_y;
};

#endif
