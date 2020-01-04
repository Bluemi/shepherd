#ifndef __FRAME_CLASS__
#define __FRAME_CLASS__

#include <vector>

#include "player.hpp"
#include "sheep.hpp"
#include "world/block_container.hpp"

class frame {
	public:
		frame();
		player* get_player(char player_id);

		bool tick();
		void check_destroy_block(player* p);
		void check_add_block(player* p);

		std::vector<player> players;
		std::vector<sheep> sheeps;
		block_container blocks;
		std::vector<glm::ivec3> block_removes;
		std::vector<glm::ivec3> block_additions;
	private:
		unsigned int _blue_win_counter;
};

#endif
