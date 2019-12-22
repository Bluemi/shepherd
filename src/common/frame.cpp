#include "frame.hpp"

frame::frame() {}

void frame::add_player(const std::string& player_name) {
	players.push_back(player(0, player_name));
}
