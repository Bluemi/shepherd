#include "actions_packet.hpp"

#include <iostream>

#include "packet_ids.hpp"
#include "packet_helper.hpp"

actions_packet::actions_packet(std::uint8_t actions, const glm::vec2& mouse_changes) : actions(actions), mouse_changes(mouse_changes) {}

actions_packet actions_packet::from_message(const std::vector<char>& buffer) {
	if (buffer[0] != packet_ids::ACTIONS_PACKET) {
		std::cerr << "invalid actions packet" << std::endl;
	}

	const char* buffer_ptr = &buffer[2];
	const char** buffer_ptr_ptr = &buffer_ptr;

	glm::vec2 mouse_changes;
	packet_helper::read_from_buffer(&mouse_changes, buffer_ptr_ptr);
	return actions_packet(buffer[1], mouse_changes);
}

void actions_packet::write_to(std::vector<char>* buffer) {
	buffer->push_back(packet_ids::ACTIONS_PACKET);
	buffer->push_back(actions);
	packet_helper::write_to_buffer(mouse_changes, buffer);
}
