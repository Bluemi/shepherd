#include "game_update_packet.hpp"

#include <iostream>

#include "../player.hpp"
#include "../sheep.hpp"
#include "packet_helper.hpp"
#include "packet_ids.hpp"

namespace packet_helper {
	template<>
	void read_from_buffer<game_update_packet::player_info>(game_update_packet::player_info* obj, const char** buffer) {
		packet_helper::read_from_buffer(&obj->id, buffer);
		packet_helper::read_from_buffer(&obj->position, buffer);
		packet_helper::read_from_buffer(&obj->view_angles, buffer);
		packet_helper::read_from_buffer(&obj->player_hook, buffer);
	}

	template<>
	void write_to_buffer(const game_update_packet::player_info& pi, std::vector<char>* buffer) {
		packet_helper::write_to_buffer(pi.id, buffer);
		packet_helper::write_to_buffer(pi.position, buffer);
		packet_helper::write_to_buffer(pi.view_angles, buffer);
		packet_helper::write_to_buffer(pi.player_hook, buffer);
	}

	template<>
	void read_from_buffer<game_update_packet::sheep_info>(game_update_packet::sheep_info* si, const char** buffer) {
		packet_helper::read_from_buffer(&si->position, buffer);
	}

	template<>
	void write_to_buffer(const game_update_packet::sheep_info& si, std::vector<char>* buffer) {
		packet_helper::write_to_buffer(si.position, buffer);
	}
}

// player info
game_update_packet::player_info::player_info() {}

game_update_packet::player_info::player_info(const player& p)
	: id(p.get_id()), position(p.get_position()), view_angles(p.get_view_angles())
{
	if (p.get_hook()) {
		player_hook = p.get_hook()->target_point;
	}
}

// sheep info
game_update_packet::sheep_info::sheep_info() {}
game_update_packet::sheep_info::sheep_info(const sheep& s) : position(s.get_position()) {}


sheep game_update_packet::sheep_info::create_sheep() const {
	return sheep(position);
}

// game update packet
game_update_packet::game_update_packet() {}

game_update_packet game_update_packet::from_game(
	const std::vector<player>& players,
	const std::vector<sheep>& sheeps,
	const std::vector<glm::ivec3>& block_removes,
	const std::vector<glm::ivec3>& block_additions
) {
	game_update_packet packet;
	for (const player& p : players) {
		packet._player_infos.push_back(game_update_packet::player_info(p));
	}

	for (const sheep& s : sheeps) {
		packet._sheep_infos.push_back(game_update_packet::sheep_info(s));
	}

	packet._block_removes.assign(block_removes.cbegin(), block_removes.cend());
	packet._block_additions.assign(block_additions.cbegin(), block_additions.cend());

	return packet;
}

game_update_packet game_update_packet::from_message(const std::vector<char>& message) {
	game_update_packet packet;

	if (message[0] != packet_ids::GAME_UPDATE_PACKET) {
		std::cerr << "wrong packet id for game_update_packet: " << (int)(message[0]) << std::endl;
	}

	const char* message_ptr = &message[1];

	packet_helper::read_from_buffer(&packet._player_infos, &message_ptr);
	packet_helper::read_from_buffer(&packet._sheep_infos, &message_ptr);
	packet_helper::read_from_buffer(&packet._block_removes, &message_ptr);
	packet_helper::read_from_buffer(&packet._block_additions, &message_ptr);

	return packet;
}

void game_update_packet::write_to(std::vector<char>* buffer) const {
	buffer->push_back(packet_ids::GAME_UPDATE_PACKET);
	packet_helper::write_to_buffer(_player_infos, buffer);
	packet_helper::write_to_buffer(_sheep_infos, buffer);
	packet_helper::write_to_buffer(_block_removes, buffer);
	packet_helper::write_to_buffer(_block_additions, buffer);
}

const std::vector<game_update_packet::player_info>& game_update_packet::get_player_infos() const {
	return _player_infos;
}

const std::vector<game_update_packet::sheep_info>& game_update_packet::get_sheep_infos() const {
	return _sheep_infos;
}

const std::vector<glm::ivec3>& game_update_packet::get_block_removes() const {
	return _block_removes;
}

const std::vector<glm::ivec3>& game_update_packet::get_block_additions() const {
	return _block_additions;
}
