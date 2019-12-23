#include <iostream>

#include <common/networking/game_update_packet.hpp>
#include <common/player.hpp>

std::ostream& operator<<(std::ostream& stream, const game_update_packet::player_info& player_info) {
	stream << "player_info(id=" << (int)(player_info.id) << " name=" << player_info.name << " position=(" << player_info.position.x << ", " << player_info.position.y << ", " << player_info.position.z << "))";
	return stream;
}

int main() {
	std::vector<player> players;
	players.push_back(player(0, "peter"));
	players.push_back(player(1, "klaus"));

	game_update_packet packet = game_update_packet::from_players(players, 42);
	std::vector<char> message;

	packet.write_to(&message);

	game_update_packet packet_copy = game_update_packet::from_message(message);

	for (unsigned int i = 0; i < packet_copy.get_player_infos().size(); i++) {
		std::cout << packet.get_player_infos()[i] << "\n" << packet_copy.get_player_infos()[i] << '\n' << std::endl;
	}
	std::cout << "local player ids: " << (int)packet.get_local_player_id() << " <-> " << (int)packet_copy.get_local_player_id() << std::endl;

	return 0;
}
