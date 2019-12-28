#include "server.hpp"

#include <stdlib.h>
#include <time.h>

#include "../common/networking/login_packet.hpp"
#include "../common/networking/actions_packet.hpp"
#include "../common/networking/game_update_packet.hpp"
#include "../common/networking/packet_ids.hpp"
#include "../common/networking/init_packet.hpp"

server::server() : _next_player_id(0) {}

void server::init() {
	_server_network_manager.run(1350);
	srand(time(NULL));
	_map_seed = rand();
	_current_frame.blocks = world_block::create_field(_map_seed);
}

void server::run() {
	std::cout << "server is running on port 1350" << std::endl;

	for (netsi::cycle c(_server_network_manager.get_context(), boost::posix_time::milliseconds(40));; c.next()) {
		check_new_peers();
		handle_clients();
		_current_frame.tick();
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
	glm::vec3 pos((rand()%10)+5, (rand()%10)+5, (rand()%10)+5);
	_current_frame.players.push_back(player(_next_player_id, p.get_player_name(), pos));
	peer_wrapper->player_id = _next_player_id;

	send_init(_next_player_id, peer_wrapper);

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

void server::handle_actions(const std::vector<char>& message, peer_wrapper* peer_wrapper) {
	actions_packet packet = actions_packet::from_message(message);
	player* current_player = _current_frame.get_player(peer_wrapper->player_id);
	current_player->set_actions(packet.actions);
	current_player->update_direction(packet.mouse_changes);
}

void server::handle_message(const std::vector<char>& message, server::peer_wrapper* peer_wrapper) {
	switch (message[0]) {
		case packet_ids::LOGIN_PACKET:
			handle_login(message, peer_wrapper);
			break;
		case packet_ids::LOGOUT_PACKET:
			handle_logout(peer_wrapper);
			break;
		case packet_ids::ACTIONS_PACKET:
			handle_actions(message, peer_wrapper);
			break;
		default:
			std::cerr << "cant handle message with id: " << (int)(message[0]) << std::endl;
			break;
	}
}

void server::handle_clients() {
	for (server::peer_wrapper& p : _peers) {
		while (!p.peer->messages().empty()) {
			std::vector<char> m = p.peer->messages().pop();
			handle_message(m, &p);
		}
	}

	_peers.erase(std::remove_if(_peers.begin(), _peers.end(), [](const server::peer_wrapper& p) { return p.disconnected; }), _peers.end());
}

void server::send_game_update() const {
	game_update_packet gup = game_update_packet::from_players(_current_frame.players);
	for (const server::peer_wrapper& p : _peers) {
		std::vector<char> buffer;
		gup.write_to(&buffer);
		p.peer->async_send(buffer);
	}
}

void server::send_init(char player_id, peer_wrapper* pw) const {
	init_packet packet(player_id, _map_seed);
	std::vector<char> buffer;
	packet.write_to(&buffer);
	pw->peer->async_send(buffer);
}

int main() {
	server svr;
	svr.init();
	svr.run();

	return 0;
}
