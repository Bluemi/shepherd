#include "actions_packet.hpp"

#include <iostream>

#include "packet_ids.hpp"

actions_packet::actions_packet(std::uint8_t actions) : actions(actions) {}

actions_packet actions_packet::from_message(const std::vector<char>& buffer) {
	if (buffer[0] != packet_ids::ACTIONS_PACKET) {
		std::cerr << "invalid actions packet" << std::endl;
	}
	return actions_packet(buffer[1]);
}

void actions_packet::write_to(std::vector<char>* buffer) {
	buffer->push_back(packet_ids::ACTIONS_PACKET);
	buffer->push_back(actions);
}
