#include "sheep.hpp"

#include "world/block_container.hpp"

constexpr float GRAVITY = 0.04f;
constexpr float SHEEP_DRAG = 0.03f;
constexpr float MAX_SHEEP_SPEED = 0.2f;
constexpr float SHEEP_ACCELERATION = 0.02f;
constexpr float SHEEP_JUMP_SPEED = 0.28f;

sheep::sheep() : _forward(0.f), _jump(false) {}

sheep::sheep(const glm::vec3& position, float yaw)
	:   _body(
			position,
			glm::vec3(0.45f, 0.5f, 0.35f),
			glm::vec3(0.01f, 0.0f, 0.0f),
			glm::vec2(0.f, yaw),
			0.3f
		),
		_forward(1.0f),
		_turn(0.4f),
		_jump(false)
{}

const glm::vec3& sheep::get_position() const {
	return _body.position;
}

float sheep::get_yaw() const {
	return _body.view_angles.y;
}

void sheep::tick(const block_container& blocks) {
	_body.speed.y -= GRAVITY;
	apply_movements(blocks);

	glm::vec3 tmp_speed(_body.speed);

	body::apply_drag(tmp_speed, SHEEP_DRAG, MAX_SHEEP_SPEED);
	_body.speed.x = tmp_speed.x; _body.speed.z = tmp_speed.z;
	_body.position += _body.speed;
	_body.physics(blocks);
}

void sheep::apply_movements(const block_container& blocks) {
	_body.view_angles.y += _turn;
	_body.speed += _body.get_direction() * (_forward * SHEEP_ACCELERATION);

	if (_jump) {
		if (!blocks.get_colliding_blocks(_body.get_bottom_collider()).empty()) {
			_body.speed.y = SHEEP_JUMP_SPEED;
		}
	}
}
