#include "sheep.hpp"

constexpr float GRAVITY = 0.04f;

sheep::sheep() {}

sheep::sheep(const glm::vec3& position) : _body(position, glm::vec3(0.45f, 0.5f, 0.35f), glm::vec3(0.01f, 0.0f, 0.0f), glm::vec2(), 0.2f) {}

const glm::vec3& sheep::get_position() const {
	return _body.position;
}

void sheep::tick(const block_container& blocks) {
	_body.speed.y -= GRAVITY;
	_body.position += _body.speed;
	_body.physics(blocks);
}
