#ifndef __SHEEP_CLASS__
#define __SHEEP_CLASS__

#include <glm/vec3.hpp>
#include "physics/body.hpp"

class sheep {
	public:
		sheep();
		sheep(const glm::vec3& position, float yaw);

		const glm::vec3& get_position() const;
		float get_yaw() const;

		void tick(const block_container& blocks);
		void apply_movements(const block_container& blocks);
	private:
		body _body;
		float _forward;
		float _turn;
		bool _jump;
};

#endif
