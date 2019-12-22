#include "server.hpp"

#include "../common/networking/login_packet.hpp"

server::server() {}

void server::init() {
	_server_network_manager.run(1350);
}

void server::run() {
	std::cout << "server is running on port 1350" << std::endl;

	for (netsi::cycle c(_server_network_manager.get_context(), boost::posix_time::milliseconds(40));; c.next()) {
		check_new_peers();
		handle_clients();
	}

	std::cout << "server is offline" << std::endl;
	_server_network_manager.join();

}

void server::check_new_peers() {
	if (!_server_network_manager.get_connecting_endpoints().empty()) {
		netsi::endpoint remote_endpoint = _server_network_manager.get_connecting_endpoints().pop();
		std::shared_ptr<netsi::peer> remote_peer = _server_network_manager.endpoint_to_peer(remote_endpoint);
		_peers.push_back(remote_peer);
		std::cout << "new peer " << remote_endpoint << std::endl;
	}
}

void server::handle_login(const std::vector<char>& login_message) {
	login_packet p = login_packet::from_message(login_message);
	_current_frame.add_player(p.get_player_name());
}

void server::handle_message(const std::vector<char>& message) {
	if (message[0] == 0) {
		handle_login(message);
	} else {
		std::cerr << "cant handle message with id: " << message[0] << std::endl;
	}
}

void server::handle_clients() {
	for (const std::shared_ptr<netsi::peer>& p : _peers) {
		if (!p->messages().empty()) {
			std::vector<char> m = p->messages().pop();
			handle_message(m);
		}
	}
}

int main() {
	server svr;
	svr.init();
	svr.run();

	return 0;
}
