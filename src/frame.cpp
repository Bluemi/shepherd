#include "frame.hpp"

frame::frame() {}

void frame::add_player(const shape& s) {
	players.push_back(player(0, "klaus", s));
}
