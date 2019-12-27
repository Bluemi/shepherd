#include "init_packet.hpp"

#include <iostream>

#include "packet_helper.hpp"
#include "packet_ids.hpp"

init_packet::init_packet() {}

init_packet::init_packet(char local_player_id, unsigned int map_seed) : local_player_id(local_player_id), map_seed(map_seed) {}

init_packet init_packet::from_message(const std::vector<char>& message) {
	if (message[0] != packet_ids::INIT_PACKET) {
		std::cerr << "wrong packet id for init packet: " << (int)(message[0]) << std::endl;
	}

	init_packet packet;

	const char* buffer_ptr = &message[1];

	packet_helper::read_from_buffer(&packet.local_player_id, &buffer_ptr);
	packet_helper::read_from_buffer(&packet.map_seed, &buffer_ptr);

	return packet;
}

void init_packet::write_to(std::vector<char>* buffer) const {
	packet_helper::write_to_buffer(packet_ids::INIT_PACKET, buffer);
	packet_helper::write_to_buffer(local_player_id, buffer);
	packet_helper::write_to_buffer(map_seed, buffer);
}
