#ifndef __LOGIN_PACKET_CLASS__
#define __LOGIN_PACKET_CLASS__

#include <string>
#include <vector>

class login_packet {
	public:
		login_packet();
		login_packet(const std::string& name);
		static login_packet from_message(const std::vector<char>& message);

		/**
		 * Writes the content of this packet into the given buffer.
		 */
		void write_to(std::vector<char>* buffer) const;

		const std::string& get_player_name() const;

	private:
		std::string _name;
};

#endif
