#ifndef __FRAME_CLASS__
#define __FRAME_CLASS__

#include <vector>

#include "player.hpp"
#include "world/world_block.hpp"

class frame {
	public:
		frame();
		player* get_player(char player_id);

		bool tick();

		std::vector<player> players;
		block_container blocks;
	private:
		unsigned int _blue_win_counter;
};

#endif
