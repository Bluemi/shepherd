#include "game_update_packet.hpp"

#include "../frame.hpp"
#include "packet_helper.hpp"

namespace packet_helper {
	template<>
	void read_from_buffer<game_update_packet::player_info>(game_update_packet::player_info* obj, const char** buffer) {
		packet_helper::read_from_buffer(&obj->id, buffer);
		packet_helper::read_from_buffer(&obj->position, buffer);
		packet_helper::read_from_buffer(&obj->name, buffer);
	}

	template<>
	void write_to_buffer(const game_update_packet::player_info& pi, std::vector<char>* buffer) {
		packet_helper::write_to_buffer(pi.id, buffer);
		packet_helper::write_to_buffer(pi.position, buffer);
		packet_helper::write_to_buffer(pi.name, buffer);
	}

}

game_update_packet::player_info::player_info() {}

game_update_packet::player_info::player_info(const player& p)
	: id(p.get_id()), position(p.get_position()), name(p.get_name())
{}

/*
void game_update_packet::player_info::write_to(std::vector<char>* buffer) const {
	packet_helper::write_to_buffer(id, buffer);
	packet_helper::write_to_buffer(position, buffer);
	packet_helper::write_to_buffer(name, buffer);
}

game_update_packet::player_info game_update_packet::player_info::read_from_message(const char** message) {
	game_update_packet::player_info pi;

	packet_helper::read_from_buffer(&pi.id, message);
	packet_helper::read_from_buffer(&pi.position, message);
	packet_helper::read_from_buffer(&pi.name, message);

	return pi;
}
*/

game_update_packet::game_update_packet() {}

game_update_packet game_update_packet::from_frame(const frame& f) {
	game_update_packet packet;
	for (const player& p : f.players) {
		packet._player_infos.push_back(game_update_packet::player_info(p));
	}
	return packet;
}

game_update_packet game_update_packet::from_message(const std::vector<char>& message) {
	game_update_packet packet;

	const char* message_ptr = &message[0];
	const char** message_ptr_ptr = &message_ptr;
	packet_helper::read_from_buffer(&packet._player_infos, message_ptr_ptr);
	return packet;
}

void game_update_packet::write_to(std::vector<char>* buffer) const {
	packet_helper::write_to_buffer(_player_infos, buffer);
}

const std::vector<game_update_packet::player_info>& game_update_packet::get_player_infos() const {
	return _player_infos;
}
