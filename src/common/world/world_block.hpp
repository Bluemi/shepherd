#ifndef __WORLD_BLOCK_CLASS__
#define __WORLD_BLOCK_CLASS__

#include <vector>
#include <glm/glm.hpp>

enum class block_type {
	VOID,
	NORMAL,
	WINNING
};

class world_block {
	public:
		world_block(const glm::ivec3& position);
		world_block(const glm::ivec3& position, const block_type block_type);

		const glm::ivec3& get_position() const;
		block_type get_type() const;

		void set_position(const glm::ivec3& position);

		bool is_winning_block() const;
	private:
		glm::ivec3 _position;
		block_type _block_type;
};

#endif
