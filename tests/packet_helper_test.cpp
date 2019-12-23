#include <iostream>

#include <common/networking/game_update_packet.hpp>
#include <common/networking/actions_packet.hpp>
#include <common/player.hpp>

std::ostream& operator<<(std::ostream& stream, const game_update_packet::player_info& player_info) {
	stream << "player_info(id=" << (int)(player_info.id) << " name=" << player_info.name <<
			  " position=(" << player_info.position.x << ", " << player_info.position.y << ", " << player_info.position.z << ")" <<
			  ", view_angles=(" << player_info.view_angles.x << ", " << player_info.view_angles.y << "))";
	return stream;
}

void test_game_update_packet() {
	std::vector<player> players;
	players.push_back(player(0, "peter"));
	players.push_back(player(1, "klaus"));

	for (player& p : players) {
		p.set_position(glm::vec3(1.3f, 5.3f, 442.f));
		p.set_view_angles(glm::vec2(1.0f, 2.1f));
	}

	game_update_packet packet = game_update_packet::from_players(players, 42);
	std::vector<char> message;

	packet.write_to(&message);

	game_update_packet packet_copy = game_update_packet::from_message(message);

	for (unsigned int i = 0; i < packet_copy.get_player_infos().size(); i++) {
		std::cout << packet.get_player_infos()[i] << "\n" << packet_copy.get_player_infos()[i] << '\n' << std::endl;
	}
	std::cout << "local player ids: " << (int)packet.get_local_player_id() << " <-> " << (int)packet_copy.get_local_player_id() << std::endl;
}

void test_actions_packet() {
	actions_packet packet(0b011001, glm::vec2(0.42f, 0.32f));

	std::vector<char> buffer;
	packet.write_to(&buffer);

	actions_packet packet_copy = actions_packet::from_message(buffer);

	std::cout << "actions: " << packet.actions << " mouse changes: " << packet.mouse_changes.x << ", " << packet.mouse_changes.y << std::endl;
	std::cout << "actions: " << packet_copy.actions << " mouse changes: " << packet_copy.mouse_changes.x << ", " << packet_copy.mouse_changes.y << std::endl;
}

int main() {
	// test_game_update_packet();
	test_actions_packet();
	return 0;
}
