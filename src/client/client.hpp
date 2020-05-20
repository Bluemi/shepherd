#ifndef __CLIENT_CLASS__
#define __CLIENT_CLASS__

#define GLM_ENABLE_EXPERIMENTAL

#include <netsi/client.hpp>

#include "../common/frame.hpp"
#include "../common/networking/game_update_packet.hpp"
#include "../common/networking/buffer_size.hpp"
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

		bool handle_message(const std::vector<char>& buffer);
		void handle_game_update(const std::vector<char>& buffer);
		void handle_player_infos(const std::vector<game_update_packet::player_info>& pis);
		void handle_sheep_infos(const std::vector<game_update_packet::sheep_info>& sis);
		void handle_block_removes(const std::vector<glm::ivec3>& block_removes);
		void handle_block_additions(const std::vector<glm::ivec3>& block_additions);
		void apply_player_info(const game_update_packet::player_info& pi);
		void handle_init(const std::vector<char>& buffer);

		frame _current_frame;
		netsi::ClientNetworkManager _network_manager;
		std::unique_ptr<renderer> _renderer;
		netsi::Peer _peer;
		std::uint16_t _last_actions;
		char _local_player_id;
};

#endif
