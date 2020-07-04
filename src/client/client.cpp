#include "client.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
#include <iostream>

#include "../common/networking/login_packet.hpp"
#include "../common/networking/init_packet.hpp"
#include "../common/networking/actions_packet.hpp"
#include "../common/networking/packet_ids.hpp"

client::client() : _network_manager(BUFFER_SIZE), _local_player_id(-1), _eye_track_server(1351, 256) {}

void client::init(const std::string& hostname, const std::string& player_name) {
	renderer::init();

	std::optional<renderer> opt_renderer = renderer::create(800, 600, "cube racing");
	if (!opt_renderer) {
		std::cerr << "Failed to initalizer renderer" << std::endl;
		return;
	}

	_renderer = std::make_unique<renderer>(*opt_renderer);

	netsi::Endpoint init_endpoint = _network_manager.resolve(hostname, 1350);
	_peer = _network_manager.create_peer(init_endpoint);

	send_login(player_name);
}

void client::run() {
	bool got_game_update_packet = false;
	while (!_renderer->should_close()) {
		if (_local_player_id != -1 && got_game_update_packet) {
			_renderer->render(_current_frame, _local_player_id);
		}

		got_game_update_packet = false;

		while (_peer.has_message()) {
			const std::vector<char> msg = _peer.pop_message();
			if (handle_message(msg)) {
				got_game_update_packet = true;
			}
		}

		send_actions_update();

		handle_eye_tracking();
	}

	send_logout();

	_renderer->close();
}

void client::send_login(const std::string& player_name) {
	login_packet packet(player_name);
	std::vector<char> buffer;
	packet.write_to(&buffer);
	_peer.send(buffer);
}

void client::send_logout() {
	std::vector<char> buffer;
	buffer.push_back(packet_ids::LOGOUT_PACKET);
	_peer.send(buffer);
}

void client::send_actions_update() {
	_renderer->get_controller().process_user_input(_renderer->get_window());
	std::uint16_t current_actions(0);
	controller& ctrl = _renderer->get_controller();
	if (ctrl.is_key_pressed(controller::CAMERA_FORWARD_KEY))
		current_actions |= FORWARD_ACTION;
	if (ctrl.is_key_pressed(controller::CAMERA_BACKWARD_KEY))
		current_actions |= BACKWARD_ACTION;
	if (ctrl.is_key_pressed(controller::CAMERA_LEFT_KEY))
		current_actions |= LEFT_ACTION;
	if (ctrl.is_key_pressed(controller::CAMERA_RIGHT_KEY))
		current_actions |= RIGHT_ACTION;
	if (ctrl.is_key_pressed(controller::CAMERA_TOP_KEY))
		current_actions |= JUMP_ACTION;
	if (ctrl.is_key_pressed(controller::CAMERA_BOTTOM_KEY))
		current_actions |= BOTTOM_ACTION;
	if (ctrl.is_left_mouse_pressed())
		current_actions |= LEFT_MOUSE_PRESSED;
	if (ctrl.is_right_mouse_pressed())
		current_actions |= RIGHT_MOUSE_PRESSED;
	if (ctrl.is_key_pressed(controller::HOOK_KEY))
		current_actions |= HOOK_ACTION;

	glm::vec2 mouse_changes = ctrl.poll_mouse_changes();

	if (_last_actions != current_actions || mouse_changes != glm::vec2()) {
		std::vector<char> buffer;
		actions_packet packet(current_actions, mouse_changes);
		packet.write_to(&buffer);
		_peer.send(buffer);
	}
	_last_actions = current_actions;
}

bool client::handle_message(const std::vector<char>& buffer) {
	bool got_game_update_packet = false;
	switch (buffer[0]) {
		case packet_ids::GAME_UPDATE_PACKET:
			handle_game_update(buffer);
			got_game_update_packet = true;
			break;
		case packet_ids::INIT_PACKET:
			handle_init(buffer);
			break;
		default:
			std::cerr << "could not handle packet with id: " << (int)(buffer[0]) << std::endl;
			break;
	}
	return got_game_update_packet;
}

void client::apply_player_info(const game_update_packet::player_info& pi) {
	bool new_player = true;
	for (player& p : _current_frame.players) {
		if (p.get_id() == pi.id) {
			p.set_position(pi.position);
			p.set_view_angles(pi.view_angles);
			p.set_hook(hook(pi.player_hook));
			new_player = false;
			break;
		}
	}

	if (new_player) {
		_current_frame.players.push_back(player(pi.id, "", pi.position, pi.player_hook));
	}
}

void client::handle_init(const std::vector<char>& buffer) {
	init_packet packet = init_packet::from_message(buffer);
	_local_player_id = packet.local_player_id;

	_current_frame.blocks = block_container(block_container::create_field(packet.map_seed));

	for (const auto& bc : _current_frame.blocks.get_chunks()) {
		_renderer->load_chunk(*(bc.second));
	}
}

void client::handle_eye_tracking() {
	while (_eye_track_server.has_client_request()) {
		netsi::ClientRequest client_request = _eye_track_server.pop_client_request();
		netsi::Peer new_peer = _eye_track_server.create_peer(client_request.endpoint);
		_eye_track_peers.push_back(new_peer);
	}

	for (netsi::Peer& peer : _eye_track_peers) {
		while (peer.has_message()) {
			const std::vector<char> message = peer.pop_message();
			if (message.size() != 12) {
				std::cerr << "got message of invalid size: " << message.size() << std::endl;
				continue;
			}
			const float* const f = reinterpret_cast<const float* const>(message.data());
			glm::vec3 v(f[0], f[1], f[2]);
			_renderer->get_camera_offset() = v;
		}
	}
}

void client::handle_game_update(const std::vector<char>& buffer) {
	game_update_packet packet = game_update_packet::from_message(buffer);
	handle_player_infos(packet.get_player_infos());
	handle_sheep_infos(packet.get_sheep_infos());
	handle_block_removes(packet.get_block_removes());
	handle_block_additions(packet.get_block_additions());
}

void client::handle_player_infos(const std::vector<game_update_packet::player_info>& player_infos) {
	std::vector<char> current_player_ids;
	for (const game_update_packet::player_info& pi : player_infos) {
		apply_player_info(pi);
		current_player_ids.push_back(pi.id);
	}

	_current_frame.players.erase(
		std::remove_if(
			_current_frame.players.begin(),
			_current_frame.players.end(),
			[current_player_ids](const player& p) { return std::find(current_player_ids.begin(), current_player_ids.end(), p.get_id()) == current_player_ids.end(); }
		),
		_current_frame.players.end()
	);

	for (auto it = _current_frame.players.begin(); it != _current_frame.players.end(); ++it) {
		bool found = false;
		for (const auto& pi : player_infos) {
			if (pi.id == it->get_id()) {
				found = true;
				break;
			}
		}
		if (!found) {
			it = _current_frame.players.erase(it);
		}
	}
}

void client::handle_sheep_infos(const std::vector<game_update_packet::sheep_info>& sis) {
	_current_frame.sheeps.clear();

	for (const game_update_packet::sheep_info& si : sis) {
		_current_frame.sheeps.push_back(si.create_sheep());
	}
}

void client::handle_block_removes(const std::vector<glm::ivec3>& block_removes) {
	for (const glm::ivec3& br : block_removes) {
		_current_frame.blocks.remove_block(br);
		_renderer->load_chunk(*_current_frame.blocks.get_containing_chunk(br));
	}
}

void client::handle_block_additions(const std::vector<glm::ivec3>& block_additions) {
	for (const glm::ivec3& ba : block_additions) {
		_current_frame.blocks.add_block(ba, block_type::NORMAL);
		_renderer->load_chunk(*_current_frame.blocks.get_containing_chunk(ba));
	}
}

void print_usage() {
	std::cout << "client [host] [playername]" << std::endl;
}


int main(int argc, const char** argv) {
	if (argc != 3) {
		print_usage();
		return 1;
	}

	std::string hostname(argv[1]);
	std::string player_name(argv[2]);
	client c;
	c.init(hostname, player_name);
	c.run();
	return 0;
}
