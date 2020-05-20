#include "server.hpp"

#include <stdlib.h>
#include <time.h>

#include "../common/networking/login_packet.hpp"
#include "../common/networking/actions_packet.hpp"
#include "../common/networking/game_update_packet.hpp"
#include "../common/networking/packet_ids.hpp"
#include "../common/networking/init_packet.hpp"
#include <netsi/util/cycle.hpp>

server::server() : _server_network_manager(1350, BUFFER_SIZE), _next_player_id(0) {}

void server::init() {
	srand(time(NULL));
	_map_seed = rand();
	_current_frame.blocks = block_container(block_container::create_field(_map_seed));
	for (unsigned int i = 0; i < 40; i++) {
		_current_frame.sheeps.push_back(sheep(_current_frame.blocks.get_sheep_respawn_position(), 0.f));
	}
}

void server::run() {
	std::cout << "server is running on port 1350" << std::endl;

	for (netsi::Cycle c(_server_network_manager.get_context(), boost::posix_time::milliseconds(40));; c.next()) {
		check_new_peers();
		handle_clients();
		_current_frame.tick();
		send_game_update();
	}

	std::cout << "server is offline" << std::endl;
}

void server::check_new_peers() {
	if (_server_network_manager.has_client_request()) {
		netsi::ClientRequest client_request = _server_network_manager.pop_client_request();
		netsi::Peer remote_peer = _server_network_manager.create_peer(client_request.endpoint);
		server::peer_wrapper new_peer_wrapper(remote_peer, -1);
		handle_login(client_request.message, &new_peer_wrapper);
		_peers.push_back(new_peer_wrapper);
	}
}

void server::handle_login(const std::vector<char>& login_message, server::peer_wrapper* peer_wrapper) {
	login_packet p = login_packet::from_message(login_message);
	_current_frame.players.push_back(player(_next_player_id, p.get_player_name(), _current_frame.blocks.get_respawn_position()));
	peer_wrapper->player_id = _next_player_id;

	send_init(_next_player_id, peer_wrapper);

	_next_player_id++;
	std::cout << "new player \"" << p.get_player_name() << "\"" << std::endl;
}

void server::handle_logout(server::peer_wrapper* peer_wrapper) {
	// peer_wrapper->peer.disconnect(); TODO
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
		while (p.peer.has_message()) {
			std::vector<char> m = p.peer.pop_message();
			handle_message(m, &p);
		}
	}

	_peers.erase(std::remove_if(_peers.begin(), _peers.end(), [](const server::peer_wrapper& p) { return p.disconnected; }), _peers.end());
}

void server::send_game_update() {
	game_update_packet gup = game_update_packet::from_game(_current_frame.players, _current_frame.sheeps, _current_frame.block_removes, _current_frame.block_additions);
	for (server::peer_wrapper& p : _peers) {
		std::vector<char> buffer;
		gup.write_to(&buffer);
		if (buffer.size() > BUFFER_SIZE) {
			std::cerr << "game update buffer size exceeded.\n\tpacketsize=" << buffer.size() << "\n\tbuffersize=" << BUFFER_SIZE << std::endl;
		} else {
			p.peer.send(buffer);
		}
	}
	_current_frame.block_removes.clear();
	_current_frame.block_additions.clear();
}

void server::send_init(char player_id, peer_wrapper* pw) const {
	init_packet packet(player_id, _map_seed);
	std::vector<char> buffer;
	packet.write_to(&buffer);
	pw->peer.send(buffer);
}

int main() {
	server svr;
	svr.init();
	svr.run();

	return 0;
}
