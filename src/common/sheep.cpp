#include "sheep.hpp"

sheep::sheep() {}

sheep::sheep(const glm::vec3& position) : _position(position) {}

const glm::vec3& sheep::get_position() const {
	return _position;
}

void sheep::tick() {
	_position.x += 0.1f;
}
