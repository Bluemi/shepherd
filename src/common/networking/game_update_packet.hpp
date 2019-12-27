#ifndef __GAME_UPDATE_PACKET_CLASS__
#define __GAME_UPDATE_PACKET_CLASS__

#include <vector>
#include <string>
#include <glm/glm.hpp>

class player;

class game_update_packet {
	public:
		struct player_info {
			player_info();
			player_info(const player& p);

			char id;
			glm::vec3 position;
			glm::vec2 view_angles;
			std::string name;
		};

		game_update_packet();
		static game_update_packet from_players(const std::vector<player>& players);
		static game_update_packet from_message(const std::vector<char>& message);

		void write_to(std::vector<char>* buffer) const;

		const std::vector<player_info>& get_player_infos() const;
	private:
		std::vector<player_info> _player_infos;
};

#endif
