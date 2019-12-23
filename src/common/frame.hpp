#ifndef __FRAME_CLASS__
#define __FRAME_CLASS__

#include <vector>

#include "player.hpp"

class frame {
	public:
		frame();
		player* get_player(char player_id);

		void tick();

		std::vector<player> players;
};

#endif
