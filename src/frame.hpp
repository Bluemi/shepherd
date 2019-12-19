#ifndef __FRAME_CLASS__
#define __FRAME_CLASS__

#include <vector>

#include "player.hpp"

class frame {
	public:
		frame();
	private:
		std::vector<player> _players;
};

#endif
