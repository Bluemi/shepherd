#ifndef __SHEEP_CLASS__
#define __SHEEP_CLASS__

#include <glm/vec3.hpp>

class sheep {
	public:
		sheep();
		sheep(const glm::vec3& position);

		const glm::vec3& get_position() const;

		void tick();
	private:
		glm::vec3 _position;
};

#endif
