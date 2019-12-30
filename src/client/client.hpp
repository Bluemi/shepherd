#ifndef __CLIENT_CLASS__
#define __CLIENT_CLASS__

#include <netsi/client/client_network_manager.hpp>

#include "../common/frame.hpp"
#include "../common/networking/game_update_packet.hpp"
#include "render/renderer.hpp"

class client {
	public:
		client();
		void init(const std::string& hostname, const std::string& player_name);
		void run();
	private:
		void send_login(const std::string& player_name);
		void send_logout();
		void send_actions_update();

		void handle_message(const std::vector<char>& buffer);
		void handle_game_update(const std::vector<char>& buffer);
		void handle_player_infos(const std::vector<game_update_packet::player_info>& pis);
		void handle_block_removes(const std::vector<glm::ivec3>& block_removes);
		void apply_player_info(const game_update_packet::player_info& pi);
		void handle_init(const std::vector<char>& buffer);

		frame _current_frame;
		netsi::client_network_manager _network_manager;
		std::unique_ptr<renderer> _renderer;
		std::shared_ptr<netsi::peer> _peer;
		std::uint8_t _last_actions;
		char _local_player_id;
};

#endif
