#ifndef __BODY_CLASS__
#define __BODY_CLASS__

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "forms.hpp"

class block_container;

class body {
	public:
		body();
		body(const glm::vec3& position, const glm::vec3& size, const glm::vec3 speed, const glm::vec2& view_angles, const float collider_dimension);

		void physics(const block_container& blocks);
		void check_collider(const block_container& blocks, const cuboid& collider, int direction, unsigned int coordinate);

		cuboid get_bottom_collider() const;
		cuboid get_top_collider() const;
		cuboid get_left_collider() const;
		cuboid get_right_collider() const;
		cuboid get_front_collider() const;
		cuboid get_back_collider() const;

		glm::vec3 position;
		glm::vec3 size;
		glm::vec3 speed;
		glm::vec2 view_angles;
		float collider_dimension;
};

#endif
