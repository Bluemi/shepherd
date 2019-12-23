#include "client.hpp"

#include "../common/networking/login_packet.hpp"
#include "../common/networking/packet_ids.hpp"

client::client() {}

void client::init(const std::string& player_name) {
	renderer::init();

	std::optional<renderer> opt_renderer = renderer::create(800, 600, "cube racing");
	if (!opt_renderer) {
		std::cerr << "Failed to initalizer renderer" << std::endl;
		return;
	}

	_renderer = std::make_unique<renderer>(*opt_renderer);

	netsi::endpoint init_endpoint = _network_manager.resolve("localhost", "1350");
	_peer = _network_manager.create_peer(init_endpoint);

	_network_manager.run();

	send_login(player_name);
}

void client::run() {
	while (!_renderer->should_close()) {
		_renderer->tick();
		_renderer->render(_current_frame);

		if (!_peer->messages().empty()) {
			const std::vector<char> msg = _peer->messages().pop();
			handle_message(msg);
		}
	}

	_renderer->close();
	_network_manager.stop();
}

void client::send_login(const std::string& player_name) {
	login_packet packet(player_name);
	std::vector<char> buffer;
	packet.write_to(&buffer);
	_peer->async_send(buffer);
}

void client::handle_message(const std::vector<char>& buffer) {
	switch (buffer[0]) {
		case packet_ids::GAME_UPDATE_PACKET:
			handle_game_update(buffer);
			break;
		default:
			std::cerr << "could not handle packet with id: " << (int)(buffer[0]) << std::endl;
	}
}

void client::apply_player_info(const game_update_packet::player_info& pi) {
	for (player& p : _current_frame.players) {
		if (p.get_id() == pi.id) {
			p.set_name(pi.name);
			p.set_position(pi.position);
			break;
		}
	}
}

void client::handle_game_update(const std::vector<char>& buffer) {
	game_update_packet packet = game_update_packet::from_message(buffer);
	for (const game_update_packet::player_info& pi : packet.get_player_infos()) {
		apply_player_info(pi);
	}
}

void print_usage() {
	std::cout << "client [playername]" << std::endl;
}


int main(int argc, const char** argv) {
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::string player_name(argv[1]);
	client c;
	c.init(player_name);
	c.run();
	return 0;
}
