#include "client.hpp"

#include "../common/networking/login_packet.hpp"

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
			std::cout << "got message" << std::endl;
			const std::vector<char> msg = _peer->messages().pop();
			std::cout.write(&msg[0], msg.size());
			std::cout << std::endl;
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
