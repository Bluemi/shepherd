#include "sheep.hpp"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/intersect.hpp>

#include "world/block_container.hpp"

constexpr float GRAVITY = 0.04f;
constexpr float SHEEP_DRAG = 0.03f;
constexpr float MAX_SHEEP_SPEED = 0.04f;
constexpr float MAX_SHEEP_SPEED_HOOKED = 0.3f;
constexpr float MAX_SHEEP_SPEED_JUMP = 0.2f;
constexpr float SHEEP_ACCELERATION = 0.03f;
constexpr float SHEEP_JUMP_ACCELERATION = 0.07f;
constexpr float SHEEP_JUMP_SPEED = 0.3f;
constexpr float TURN_SPEED = 0.15f;
constexpr unsigned int JUMP_DURATION = 5;

sheep::sheep() : _state(sheep_state::WAIT), _state_counter(0), _forward(0.f), _jump(0), _is_hooked(false) {}

sheep::sheep(const glm::vec3& position, float yaw)
	:   _body(
			position,
			glm::vec3(0.45f, 0.5f, 0.35f),
			glm::vec3(0.01f, 0.0f, 0.0f),
			glm::vec2(0.f, yaw),
			0.3f
		),
		_state(sheep_state::WAIT),
		_state_counter(0),
		_forward(1.0f),
		_turn(0.4f),
		_jump(0),
		_is_hooked(false)
{}

const glm::vec3& sheep::get_position() const {
	return _body.position;
}

float sheep::get_yaw() const {
	return _body.view_angles.y;
}

bool sheep::is_hooked() const {
	return _is_hooked;
}

void sheep::set_is_hooked(bool h) {
	_is_hooked = h;
}

bool sheep::is_colliding(const ray& r, float range) const {
	glm::vec3 intersection_position, intersection_normal, intersection_position2, intersection_normal2;
	bool intersect = glm::intersectLineSphere(r.position, r.position + glm::normalize(r.direction) * range, _body.position, 0.5f, intersection_position, intersection_normal, intersection_position2, intersection_normal2);
	if (intersect) {
		return glm::dot(r.direction, intersection_position - r.position) > 0.f;
	}
	return false;
}

void sheep::accelerate(const glm::vec3& acceleration) {
	_body.speed += acceleration;
}

void sheep::tick(const block_container& blocks) {
	if (_is_hooked) {
		_body.speed.y -= GRAVITY*0.4f;
	} else {
		_body.speed.y -= GRAVITY;
	}
	think(blocks);
	apply_movements(blocks);

	glm::vec3 tmp_speed(_body.speed);

	float max_sheep_speed = MAX_SHEEP_SPEED;
	float sheep_drag = SHEEP_DRAG;
	if (_is_hooked) {
		max_sheep_speed = MAX_SHEEP_SPEED_HOOKED;
		sheep_drag *= 0.2f;
	} else if (_jump) {
		max_sheep_speed = MAX_SHEEP_SPEED_JUMP;
	}

	body::apply_drag(tmp_speed, sheep_drag, max_sheep_speed);
	if (_is_hooked) {
		_body.speed = tmp_speed;
	} else {
		_body.speed.x = tmp_speed.x;
		_body.speed.z = tmp_speed.z;
	}
	_body.position += _body.speed;
	_body.physics(blocks);

	if (_body.position.y < blocks.get_min_y() - 100.f) {
		respawn(blocks);
	}
}

void sheep::apply_movements(const block_container& blocks) {
	_body.view_angles.y += _turn;
	float sheep_acceleration = SHEEP_ACCELERATION;
	if (_jump) {
		sheep_acceleration = SHEEP_JUMP_ACCELERATION;
	}
	_body.speed += _body.get_direction() * (_forward * sheep_acceleration);

	if (_jump) {
		if (_jump == JUMP_DURATION) {
			if (!blocks.get_colliding_blocks(_body.get_bottom_collider()).empty()) {
				_body.speed.y = SHEEP_JUMP_SPEED;
			}
		}
		_jump--;
	}
}

void sheep::respawn(const block_container& blocks) {
	_body.position = blocks.get_sheep_respawn_position();
	_body.speed = glm::vec3();
}

void sheep::think(const block_container& blocks) {
	switch (_state) {
		case sheep_state::WAIT:
			wait();
			break;
		case sheep_state::MOVE:
			move(blocks);
			break;
		case sheep_state::TURN:
			turn();
			break;
		default:
			std::cout << "unknown sheep state" << std::endl;
			break;
	}

	if (_body.position.x < 5.f && _body.get_direction().x < 0.f) {
		reset_state_counter();
		_turn = 1.f;
		_forward = 0.f;
		_state = sheep_state::TURN;
	} else if (_body.position.x > MAP_X_SIZE - 5.f && _body.get_direction().x > 0.f) {
		reset_state_counter();
		_turn = 1.f;
		_forward = 0.f;
		_state = sheep_state::TURN;
	} else if (_body.position.z < 5.f && _body.get_direction().z < 0.f) {
		reset_state_counter();
		_turn = 1.f;
		_forward = 0.f;
		_state = sheep_state::TURN;
	} else if (_body.position.z > MAP_Z_SIZE - 5.f && _body.get_direction().z > 0.f) {
		reset_state_counter();
		_turn = 1.f;
		_forward = 0.f;
		_state = sheep_state::TURN;
	}
	_state_counter--;
}

void sheep::wait() {
	if (_state_counter <= 0) {
		start_move();
	}
}

void sheep::move(const block_container& blocks) {
	if (_state_counter <= 0) {
		start_turn();
	} else {
		std::optional<world_block> front_block = blocks.get_colliding_block(ray(_body.position, _body.get_direction()), 0.7f);
		if (front_block) {
			_jump = JUMP_DURATION;
		}
	}
}

void sheep::turn() {
	if (_state_counter <= 0) {
		start_wait();
	}
}

void sheep::start_wait() {
	reset_state_counter();
	_turn = 0.f;
	_forward = 0.f;
	_state = sheep_state::WAIT;
}

void sheep::start_move() {
	reset_state_counter();
	_turn = 0.f;
	_forward = 1.f;
	_state = sheep_state::MOVE;
}

void sheep::start_turn() {
	reset_state_counter();
	_turn = (rand() % 2) * 2 - 1;
	_forward = 0.f;
	_state = sheep_state::TURN;
}

void sheep::reset_state_counter() {
	_state_counter = 20 + rand() % 15;
}
