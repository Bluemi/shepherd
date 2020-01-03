#ifndef __GAME_UPDATE_PACKET_CLASS__
#define __GAME_UPDATE_PACKET_CLASS__

#include <vector>
#include <string>
#include <optional>
#include <glm/glm.hpp>

#include "../hook.hpp"

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

			std::optional<glm::vec3> player_hook;
		};

		game_update_packet();
		static game_update_packet from_game(const std::vector<player>& players, const std::vector<glm::ivec3>& block_removes, const std::vector<glm::ivec3>& block_additions);
		static game_update_packet from_message(const std::vector<char>& message);

		void write_to(std::vector<char>* buffer) const;

		const std::vector<player_info>& get_player_infos() const;
		const std::vector<glm::ivec3>& get_block_removes() const;
		const std::vector<glm::ivec3>& get_block_additions() const;
	private:
		std::vector<player_info> _player_infos;
		std::vector<glm::ivec3> _block_removes;
		std::vector<glm::ivec3> _block_additions;
};

#endif
