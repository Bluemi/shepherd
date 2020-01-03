#ifndef __ACTIONS_PACKET_CLASS__
#define __ACTIONS_PACKET_CLASS__

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

constexpr std::uint16_t FORWARD_ACTION = 	  1 << 0;
constexpr std::uint16_t BACKWARD_ACTION = 	  1 << 1;
constexpr std::uint16_t LEFT_ACTION = 		  1 << 2;
constexpr std::uint16_t RIGHT_ACTION =		  1 << 3;
constexpr std::uint16_t JUMP_ACTION = 		  1 << 4;
constexpr std::uint16_t BOTTOM_ACTION = 	  1 << 5;
constexpr std::uint16_t LEFT_MOUSE_PRESSED =  1 << 6;
constexpr std::uint16_t RIGHT_MOUSE_PRESSED = 1 << 7;
constexpr std::uint16_t HOOK_ACTION =         1 << 8;

class actions_packet {
	public:
		actions_packet();
		actions_packet(std::uint16_t actions, const glm::vec2& mouse_changes);
		static actions_packet from_message(const std::vector<char>& buffer);

		void write_to(std::vector<char>* buffer);

		std::uint16_t actions;
		glm::vec2 mouse_changes;
};

#endif
