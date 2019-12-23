#include "login_packet.hpp"

#include <iostream>

#include "packet_ids.hpp"

login_packet::login_packet() {}
login_packet::login_packet(const std::string& name) : _name(name) {}

login_packet login_packet::from_message(const std::vector<char>& message) {
	if (message[0] != packet_ids::LOGIN_PACKET) {
		std::cerr << "invalid login message" << std::endl;
	}
	return login_packet(std::string(message.cbegin()+1, message.cend()));
}

void login_packet::write_to(std::vector<char>* buffer) const {
	buffer->push_back(packet_ids::LOGIN_PACKET); // packet type
	buffer->insert(buffer->end(), _name.cbegin(), _name.cend());
}

const std::string& login_packet::get_player_name() const {
	return _name;
}
