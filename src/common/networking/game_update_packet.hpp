#ifndef __GAME_UPDATE_PACKET_CLASS__
#define __GAME_UPDATE_PACKET_CLASS__

#include <vector>
#include <string>
#include <glm/glm.hpp>

class frame;
class player;

class game_update_packet {
	public:
		struct player_info {
			player_info();
			player_info(const player& p);

			/*
			void write_to(std::vector<char>* buffer) const;
			static player_info read_from_message(const char** message);
			*/

			char id;
			glm::vec3 position;
			std::string name;
		};

		game_update_packet();
		static game_update_packet from_frame(const frame& frame);
		static game_update_packet from_message(const std::vector<char>& message);

		void write_to(std::vector<char>* buffer) const;

		const std::vector<player_info>& get_player_infos() const;
	private:
		std::vector<player_info> _player_infos;
};

#endif
