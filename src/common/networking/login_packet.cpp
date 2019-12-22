#include "login_packet.hpp"

#include <iostream>

login_packet::login_packet() {}
login_packet::login_packet(const std::string& name) : _name(name) {}

login_packet login_packet::from_message(const std::vector<char>& message) {
	if (message[0] != 0) {
		std::cerr << "invalid login message" << std::endl;
	}
	return login_packet(std::string(message.cbegin()+1, message.cend()));
}

void login_packet::write_to(std::vector<char>* buffer) const {
	buffer->push_back(0); // packet type
	buffer->insert(buffer->end(), _name.cbegin(), _name.cend());
}

const std::string& login_packet::get_player_name() const {
	return _name;
}
