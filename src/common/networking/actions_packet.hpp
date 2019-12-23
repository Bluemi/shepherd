#ifndef __ACTIONS_PACKET_CLASS__
#define __ACTIONS_PACKET_CLASS__

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

constexpr std::uint8_t FORWARD_ACTION = 	0b1;
constexpr std::uint8_t BACKWARD_ACTION = 	0b10;
constexpr std::uint8_t LEFT_ACTION = 		0b100;
constexpr std::uint8_t RIGHT_ACTION = 		0b1000;
constexpr std::uint8_t JUMP_ACTION = 		0b10000;
constexpr std::uint8_t BOTTOM_ACTION = 		0b100000;

class actions_packet {
	public:
		actions_packet(std::uint8_t actions, const glm::vec2& mouse_changes);
		static actions_packet from_message(const std::vector<char>& buffer);

		void write_to(std::vector<char>* buffer);

		std::uint16_t actions;
		glm::vec2 mouse_changes;
};

#endif
