#include "server.hpp"

#include <stdlib.h>
#include <time.h>

#include "../common/networking/login_packet.hpp"
#include "../common/networking/game_update_packet.hpp"
#include "../common/networking/packet_ids.hpp"

server::server() {}

void server::init() {
	_server_network_manager.run(1350);
	srand(time(NULL));
}

void server::run() {
	std::cout << "server is running on port 1350" << std::endl;

	for (netsi::cycle c(_server_network_manager.get_context(), boost::posix_time::milliseconds(40));; c.next()) {
		check_new_peers();
		handle_clients();
		send_game_update();
	}

	std::cout << "server is offline" << std::endl;
	_server_network_manager.join();

}

void server::check_new_peers() {
	if (!_server_network_manager.get_connecting_endpoints().empty()) {
		netsi::endpoint remote_endpoint = _server_network_manager.get_connecting_endpoints().pop();
		std::shared_ptr<netsi::peer> remote_peer = _server_network_manager.endpoint_to_peer(remote_endpoint);
		_peers.push_back(server::peer_wrapper(remote_peer, -1));
	}
}

void server::handle_login(const std::vector<char>& login_message, server::peer_wrapper* peer_wrapper) {
	login_packet p = login_packet::from_message(login_message);
	glm::vec3 pos((rand()%10)-5, (rand()%10)-5, (rand()%10)-5);
	_current_frame.players.push_back(player(_next_player_id, p.get_player_name(), pos));
	peer_wrapper->player_id = _next_player_id;
	_next_player_id++;
	std::cout << "new player \"" << p.get_player_name() << "\"" << std::endl;
}

void server::handle_logout(server::peer_wrapper* peer_wrapper) {
	peer_wrapper->peer->disconnect();
	peer_wrapper->disconnected = true;

	for (auto it = _current_frame.players.begin(); it != _current_frame.players.end(); ++it) {
		if (it->get_id() == peer_wrapper->player_id) {
			std::cout << "player \"" << it->get_name() << "\" disconnected" << std::endl;
			it = _current_frame.players.erase(it);
			break;
		}
	}
}

void server::handle_message(const std::vector<char>& message, server::peer_wrapper* peer_wrapper) {
	switch (message[0]) {
		case packet_ids::LOGIN_PACKET:
			handle_login(message, peer_wrapper);
			break;
		case packet_ids::LOGOUT_PACKET:
			handle_logout(peer_wrapper);
			break;
		default:
			std::cerr << "cant handle message with id: " << (int)(message[0]) << std::endl;
			break;
	}
}

void server::handle_clients() {
	for (server::peer_wrapper& p : _peers) {
		if (!p.peer->messages().empty()) {
			std::vector<char> m = p.peer->messages().pop();
			handle_message(m, &p);
		}
	}

	_peers.erase(std::remove_if(_peers.begin(), _peers.end(), [](const server::peer_wrapper& p) { return p.disconnected; }), _peers.end());
}

void server::send_game_update() const {
	game_update_packet gup = game_update_packet::from_players(_current_frame.players);
	std::vector<char> buffer;
	gup.write_to(&buffer);
	for (const server::peer_wrapper& p : _peers) {
		p.peer->async_send(buffer);
	}
}

int main() {
	server svr;
	svr.init();
	svr.run();

	return 0;
}
