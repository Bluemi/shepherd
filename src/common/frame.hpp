#ifndef __FRAME_CLASS__
#define __FRAME_CLASS__

#include <vector>

#include "player.hpp"

class frame {
	public:
		frame();

		void add_player();

		std::vector<player> players;
};

#endif
