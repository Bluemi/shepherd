#ifndef __WORLD_BLOCK_CLASS__
#define __WORLD_BLOCK_CLASS__

#include <glm/glm.hpp>

class world_block {
	public:
		world_block(const glm::vec3& position);

		const glm::vec3& get_position() const;
		const glm::vec3& get_color() const;

		void set_position(const glm::vec3& position);
		void set_color(const glm::vec3& color);
	private:
		glm::vec3 _position;
		glm::vec3 _color;
};

#endif
