#include "init_packet.hpp"

#include <iostream>

#include "packet_helper.hpp"
#include "packet_ids.hpp"

init_packet::init_packet(char local_player_id) : local_player_id(local_player_id) {}

init_packet init_packet::from_message(const std::vector<char>& message) {
	if (message[0] != packet_ids::INIT_PACKET) {
		std::cerr << "wrong packet id for init packet: " << (int)(message[0]) << std::endl;
	}

	return init_packet(message[1]);
}

void init_packet::write_to(std::vector<char>* buffer) const {
	packet_helper::write_to_buffer(packet_ids::INIT_PACKET, buffer);
	packet_helper::write_to_buffer(local_player_id, buffer);
}
