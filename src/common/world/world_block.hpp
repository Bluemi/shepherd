#ifndef __WORLD_BLOCK_CLASS__
#define __WORLD_BLOCK_CLASS__

#include <vector>
#include <glm/glm.hpp>

enum class block_type {
	NORMAL,
	WINNING
};

class world_block {
	public:
		world_block(const glm::vec3& position);
		world_block(const glm::vec3& position, const glm::vec3& color);
		world_block(const glm::vec3& position, const glm::vec3& color, const block_type& block_type);

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

#endif
