#include "frame.hpp"

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
	}

	return _blue_win_counter >= WIN_LIMIT;
}
