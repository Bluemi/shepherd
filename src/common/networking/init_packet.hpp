#ifndef __INIT_PACKET_CLASS__
#define __INIT_PACKET_CLASS__

#include <vector>

class init_packet {
	public:
		init_packet();
		init_packet(char local_player_id, unsigned int map_seed);
		static init_packet from_message(const std::vector<char>& message);
		void write_to(std::vector<char>* buffer) const;

		char local_player_id;
		unsigned int map_seed;
};

#endif
