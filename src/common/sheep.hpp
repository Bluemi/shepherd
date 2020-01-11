#ifndef __SHEEP_CLASS__
#define __SHEEP_CLASS__

#include <glm/vec3.hpp>
#include "physics/body.hpp"

class sheep {
	public:
		sheep();
		sheep(const glm::vec3& position);

		const glm::vec3& get_position() const;

		void tick(const block_container& blocks);
	private:
		body _body;
};

#endif
