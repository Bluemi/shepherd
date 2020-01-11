#ifndef __SHEEP_CLASS__
#define __SHEEP_CLASS__

#include <optional>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "physics/body.hpp"

enum class sheep_state {
	WAIT,
	MOVE,
	TURN
};

class sheep {
	public:
		sheep();
		sheep(const glm::vec3& position, float yaw);

		const glm::vec3& get_position() const;
		float get_yaw() const;
		bool is_hooked() const;

		void set_is_hooked(bool h);

		bool is_colliding(const ray& r, float range) const;

		void accelerate(const glm::vec3& acceleration);
		void tick(const block_container& blocks);
		void apply_movements(const block_container& blocks);
		void think(const block_container& blocks);

		void wait();
		void move(const block_container& blocks);
		void turn();

		void start_wait();
		void start_move();
		void start_turn();

		void reset_state_counter();
	private:
		body _body;
		sheep_state _state;
		int _state_counter;

		float _forward;
		float _turn;
		bool _jump;
		bool _is_hooked;
};

#endif
