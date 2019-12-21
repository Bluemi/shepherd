#include "server.hpp"

server::server() {}

void server::init() {
	_server_network_manager.run(1350);
}

void server::run() {
	std::cout << "server is running on port 1350" << std::endl;

	for (netsi::cycle c(_server_network_manager.get_context(), boost::posix_time::milliseconds(40));; c.next()) {
		check_new_peers();
		handle_to_clients();
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

void server::handle_to_clients() {
	unsigned char client_id = 0;
	for (const std::shared_ptr<netsi::peer>& p : _peers) {
		if (!p->messages().empty()) {
			std::vector<char> m = p->messages().pop();
			m.push_back(' ');
			m.push_back('0' + client_id);

			for (std::shared_ptr<netsi::peer>& prs : _peers) {
				prs->async_send(m);
			}
		}

		client_id++;
	}
}

int main() {
	server svr;
	svr.init();
	svr.run();

	return 0;
}
