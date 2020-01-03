#include "actions_packet.hpp"

#include <iostream>

#include "packet_ids.hpp"
#include "packet_helper.hpp"

actions_packet::actions_packet() {}

actions_packet::actions_packet(std::uint16_t actions, const glm::vec2& mouse_changes) : actions(actions), mouse_changes(mouse_changes) {}

actions_packet actions_packet::from_message(const std::vector<char>& buffer) {
	if (buffer[0] != packet_ids::ACTIONS_PACKET) {
		std::cerr << "invalid actions packet" << std::endl;
	}

	actions_packet packet;

	const char* buffer_ptr = &buffer[1];

	packet_helper::read_from_buffer(&packet.actions, &buffer_ptr);
	packet_helper::read_from_buffer(&packet.mouse_changes, &buffer_ptr);
	return packet;
}

void actions_packet::write_to(std::vector<char>* buffer) {
	buffer->push_back(packet_ids::ACTIONS_PACKET);
	packet_helper::write_to_buffer(actions, buffer);
	packet_helper::write_to_buffer(mouse_changes, buffer);
}
