#include "frame.hpp"

#include <iostream>

#include "physics/util.hpp"

constexpr unsigned int WIN_LIMIT = 2;
constexpr float BUILD_RANGE = 1000.f;
constexpr float DESTROY_RANGE = 1000.f;

frame::frame() {}

player* frame::get_player(char player_id) {
	player* a_player = nullptr;
	for (player& p : players) {
		if (p.get_id() == player_id) {
			a_player = &p;
			break;
		}
	}
	return a_player;
}

bool frame::tick() {
	for (player& p : players) {
		if (p.tick(blocks)) {
			_blue_win_counter++;
		}
		check_destroy_block(&p);
		check_add_block(&p);
	}

	return _blue_win_counter >= WIN_LIMIT;
}

void frame::check_destroy_block(player* p) {
	if (p->poll_left_mouse_pressed()) {
		std::optional<world_block> block_to_destroy = blocks.get_colliding_block(ray(p->get_camera_position(), p->get_direction()), DESTROY_RANGE);
		if (block_to_destroy && world_block::destroyable(block_to_destroy->get_type())) {
			blocks.remove_block(block_to_destroy->get_position());
			block_removes.push_back(block_to_destroy->get_position());
		}
	}
}

void frame::check_add_block(player* p) {
	if (p->poll_right_mouse_pressed()) {
		std::optional<glm::ivec3> block_placement_position = blocks.get_addition_position(ray(p->get_camera_position(), p->get_direction()), BUILD_RANGE);
		if (block_placement_position) {
			blocks.add_block(*block_placement_position, block_type::NORMAL);
			block_additions.push_back(*block_placement_position);
		}
	}
}
