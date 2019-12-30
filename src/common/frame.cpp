#include "frame.hpp"

#include <iostream>

#include "physics/util.hpp"

constexpr unsigned int WIN_LIMIT = 2;

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
	}

	return _blue_win_counter >= WIN_LIMIT;
}

void frame::check_destroy_block(player* p) {
	if (p->poll_left_mouse_pressed()) {
		std::optional<world_block> block_to_destroy = blocks.get_colliding_block(ray(p->get_camera_position(), p->get_direction()), 3.f);
		if (block_to_destroy) {
			blocks.remove_block(block_to_destroy->get_position());
			block_removes.push_back(block_to_destroy->get_position());
		}
	}
}
