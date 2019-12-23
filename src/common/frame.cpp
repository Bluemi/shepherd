#include "frame.hpp"

frame::frame() {}

player* frame::get_player(char player_id) {
	player* a_player;
	for (player& p : players) {
		if (p.get_id() == player_id) {
			a_player = &p;
		}
	}
	return a_player;
}

void frame::tick() {
	for (player& p : players) {
		p.tick();
	}
}
