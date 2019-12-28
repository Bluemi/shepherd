#ifndef __WORLD_BLOCK_CLASS__
#define __WORLD_BLOCK_CLASS__

#include <vector>
#include <glm/glm.hpp>

#include "../physics/forms.hpp"

enum class block_type {
	NORMAL,
	WINNING
};

class world_block {
	public:
		world_block(const glm::vec3& position);
		world_block(const glm::vec3& position, const glm::vec3& color);
		world_block(const glm::vec3& position, const glm::vec3& color, const block_type& block_type);

		static std::vector<world_block> create_field(unsigned int seed);

		const glm::vec3& get_position() const;
		const glm::vec3& get_color() const;

		void set_position(const glm::vec3& position);
		void set_color(const glm::vec3& color);

		bool is_winning_block() const;
	private:
		glm::vec3 _position;
		glm::vec3 _color;
		block_type _block_type;
};

class block_container {
	public:
		block_container();
		block_container(const std::vector<world_block>& blocks);

		glm::vec3 get_respawn_position() const;

		const std::vector<world_block>& get_blocks() const;
		std::vector<world_block>& get_blocks();
		world_block& get_block(unsigned int x, unsigned int z);
		const world_block& get_block(unsigned int x, unsigned int z) const;

		// std::vector<world_block*> get_colliding_blocks(const cuboid&);
		std::vector<const world_block*> get_colliding_blocks(const cuboid&) const;
	private:
		std::vector<world_block> _blocks;
};

#endif
