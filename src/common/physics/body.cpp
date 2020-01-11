#include "body.hpp"

#include "../world/block_container.hpp"

body::body() {}

body::body(const glm::vec3& position, const glm::vec3& size, const glm::vec3 speed, const glm::vec2& view_angles, const float collider_dimension)
	: position(position), size(size), speed(speed), view_angles(view_angles), collider_dimension(collider_dimension)
{}

void body::physics(const block_container& blocks) {
	bool y_checked = false;
	if (glm::abs(speed.y) > 0.2f) {
		check_collider(blocks, get_bottom_collider(), -1, 1);
		check_collider(blocks, get_top_collider()   ,  1, 1);
		y_checked = true;
	}
	check_collider(blocks, get_left_collider()  , -1, 2);
	check_collider(blocks, get_right_collider() ,  1, 2);
	check_collider(blocks, get_back_collider()  , -1, 0);
	check_collider(blocks, get_front_collider() ,  1, 0);

	if (!y_checked) {
		check_collider(blocks, get_bottom_collider(), -1, 1);
		check_collider(blocks, get_top_collider()   ,  1, 1);
	}
}

// direction = -1, if block is in negative direction to player
void body::check_collider(const block_container& blocks, const cuboid& collider, int direction, unsigned int coordinate) {
	std::vector<world_block> colliding_blocks = blocks.get_colliding_blocks(collider);
	if (!colliding_blocks.empty()) {
		if (speed[coordinate]*direction > 0.f) {
			speed[coordinate] = 0.f;
		}
		float min_coord = colliding_blocks[0].get_position()[coordinate]*direction;
		for (const world_block& wb : colliding_blocks) {
			min_coord = glm::min(min_coord, static_cast<float>(wb.get_position()[coordinate]*direction));
		}
		position[coordinate] = (min_coord*direction) - (0.5f + size.y - 0.01f)*direction;
	}
}

cuboid body::get_bottom_collider() const {
	return cuboid(
		glm::vec3(position.x, position.y-0.4f, position.z),
		glm::vec3(collider_dimension, 0.1f, collider_dimension)
	);
}

cuboid body::get_top_collider() const {
	return cuboid(
		glm::vec3(position.x, position.y+0.4f, position.z),
		glm::vec3(collider_dimension, 0.1f, collider_dimension)
	);
}

cuboid body::get_left_collider() const {
	return cuboid(
		glm::vec3(position.x, position.y, position.z-0.4f),
		glm::vec3(collider_dimension, collider_dimension, 0.1f)
	);
}

cuboid body::get_right_collider() const {
	return cuboid(
		glm::vec3(position.x, position.y, position.z+0.4f),
		glm::vec3(collider_dimension, collider_dimension, 0.1f)
	);
}

cuboid body::get_front_collider() const {
	return cuboid(
		glm::vec3(position.x+0.4f, position.y, position.z),
		glm::vec3(0.1f, collider_dimension, collider_dimension)
	);
}

cuboid body::get_back_collider() const {
	return cuboid(
		glm::vec3(position.x-0.4f, position.y, position.z),
		glm::vec3(0.1f, collider_dimension, collider_dimension)
	);
}
