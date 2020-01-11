#include "player.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "networking/actions_packet.hpp"
#include "physics/util.hpp"
#include "sheep.hpp"

const float PLAYER_ROTATE_SPEED = 0.05f;
const glm::vec3 CAMERA_OFFSET = glm::vec3(0, 0.4f, 0);
constexpr float GRAVITY = 0.04f;
constexpr float PLAYER_JUMP_SPEED = 0.28f;
constexpr float PLAYER_COLLIDER_DIMENSION = 0.2f;
constexpr float PLAYER_DRAG = 0.03f;
constexpr float MAX_PLAYER_SPEED = 0.2f;
constexpr unsigned int NUM_BLOCKS_TO_PLACE = 20;
constexpr unsigned int NUM_BLOCKS_TO_DESTROY = 20;
constexpr float HOOK_DRAG = 0.7f;
constexpr glm::vec3 PLAYER_SIZE = glm::vec3(0.5f, 0.5f, 0.5f);

player::player(unsigned int id, const std::string& name)
	: _id(id), _name(name), _body(glm::vec3(), PLAYER_SIZE, glm::vec3(), glm::vec2(), PLAYER_COLLIDER_DIMENSION), _color(0.1, 0.1, 0.4), _on_left_mouse_pressed(false), _on_right_mouse_pressed(false)
{}

player::player(unsigned int id, const std::string& name, const glm::vec3& position)
	: _id(id), _name(name), _body(position, PLAYER_SIZE, glm::vec3(), glm::vec2(), PLAYER_COLLIDER_DIMENSION), _color(0.02, 0.02, 0.2), _on_left_mouse_pressed(false), _on_right_mouse_pressed(false)
{}

player::player(unsigned int id, const std::string& name, const glm::vec3& position, const std::optional<glm::vec3>& h)
	: _id(id), _name(name), _body(position, PLAYER_SIZE, glm::vec3(), glm::vec2(), PLAYER_COLLIDER_DIMENSION), _color(0.02, 0.02, 0.2), _on_left_mouse_pressed(false), _on_right_mouse_pressed(false), _hook(h)
{}

char player::get_id() const {
	return _id;
}

const std::string& player::get_name() const {
	return _name;
}

const glm::vec3& player::get_position() const {
	return _body.position;
}

const glm::vec2& player::get_view_angles() const {
	return _body.view_angles;
}

const glm::vec3& player::get_speed() const {
	return _body.speed;
}

std::uint16_t player::get_actions() const {
	return _actions;
}

bool player::poll_left_mouse_pressed() {
	bool lmp = _on_left_mouse_pressed;
	_on_left_mouse_pressed = false;
	return lmp;
}

bool player::poll_right_mouse_pressed() {
	bool rmp = _on_right_mouse_pressed;
	_on_right_mouse_pressed = false;
	return rmp;
}

glm::vec3 player::get_color() const {
	return _color;
}

const std::optional<hook>& player::get_hook() const {
	return _hook;
}

bool player::is_hooked() const {
	return _hook && _hook->is_hooked();
}

bool player::is_block_hooked() const {
	return _hook && _hook->target_point;
}

void player::set_name(const std::string& name) {
	_name = name;
}

void player::set_position(const glm::vec3& position) {
	_body.position = position;
}

void player::set_view_angles(const glm::vec2& view_angles) {
	_body.view_angles = view_angles;
}

void player::set_speed(const glm::vec3& speed) {
	_body.speed = speed;
}

void player::set_actions(const std::uint16_t actions) {
	if (actions & LEFT_MOUSE_PRESSED && !(_actions & LEFT_MOUSE_PRESSED)) {
		_on_left_mouse_pressed = true;
	}
	if (actions & RIGHT_MOUSE_PRESSED && !(_actions & RIGHT_MOUSE_PRESSED)) {
		_on_right_mouse_pressed = true;
	}
	_actions = actions;
}

void player::set_hook(const std::optional<hook>& h) {
	_hook = h;
}

void player::update_direction(const glm::vec2& direction_update) {
	_body.view_angles.y += direction_update.x * PLAYER_ROTATE_SPEED;
	_body.view_angles.x -= direction_update.y * PLAYER_ROTATE_SPEED;
	_body.view_angles.x = fmax(fmin(_body.view_angles.x, 89.f), -89.0f);
}

glm::vec3 player::get_right() const {
	return _body.get_right();
}

glm::vec3 player::get_direction() const {
	return _body.get_direction();
}

glm::vec3 player::get_top() const {
	return _body.get_top();
}

glm::mat4 player::get_look_at() const {
	return glm::lookAt(get_camera_position(), _body.position + CAMERA_OFFSET + get_direction(), body::get_up());
}

glm::vec3 player::get_camera_position() const {
	return _body.position + CAMERA_OFFSET;
}

void player::respawn(const glm::vec3& position) {
	_body.position = position;
	_body.speed = glm::vec3();
	_body.view_angles = glm::vec2();
	_hook.reset();
}

bool player::tick(const block_container& blocks, std::vector<sheep>& sheeps) {
	if (!(_hook && _hook->target_point)) {
		_body.speed.y -= GRAVITY;
	}
	apply_player_movements(blocks);
	_body.position += _body.speed;

	handle_hook(blocks, sheeps);
	_body.physics(blocks);

	bool was_winning = false;

	for (const world_block& wb : blocks.get_colliding_blocks(_body.get_bottom_collider())) {
		if (wb.is_winning_block()) {
			respawn(blocks.get_respawn_position());
			was_winning = true;
		}
	}

	if (_body.position.y < blocks.get_min_y() - 100.f) {
		respawn(blocks.get_respawn_position());
	}

	return was_winning;
}

void player::apply_player_movements(const block_container& blocks) {
	float forward = 0;
	if (_actions & FORWARD_ACTION)
		forward++;
	if (_actions & BACKWARD_ACTION)
		forward--;

	float right = 0;
	if (_actions & LEFT_ACTION)
		right--;
	if (_actions & RIGHT_ACTION)
		right++;

	if (_actions & JUMP_ACTION) {
		if (!blocks.get_colliding_blocks(_body.get_bottom_collider()).empty()) {
			_body.speed.y = PLAYER_JUMP_SPEED;
		}
	}

	glm::vec3 tmp_direction = glm::normalize(glm::vec3(get_direction().x, 0.f, get_direction().z));

	_body.speed += (get_right()*right + tmp_direction*forward)*0.1f;

	glm::vec3 tmp_speed = _body.speed;

	if (is_block_hooked()) {
		tmp_speed *= HOOK_DRAG;
		_body.speed = tmp_speed;
	} else {
		body::apply_drag(tmp_speed, PLAYER_DRAG, MAX_PLAYER_SPEED);
		_body.speed.x = tmp_speed.x;
		_body.speed.z = tmp_speed.z;
	}
}

void player::physics(const block_container& blocks) {
	_body.physics(blocks);
}

void player::handle_active_hook(const block_container& blocks, std::vector<sheep>& sheeps) {
	if (!_hook->is_hooked()) {
		_hook->range += HOOK_SPEED;
		_hook->check_target(blocks, sheeps);
		if ((!_hook->is_hooked()) && _hook->range >= HOOK_RANGE) {
			_hook.reset();
		}
	}

	if (_hook) {
		if (_hook->target_point) {
			const glm::vec3 hook_direction = glm::normalize(*(_hook->target_point) - _body.position);
			_body.speed += hook_direction*HOOK_ACCELERATION;
		} else if (_hook->target_sheep_index) {
			sheep& target_sheep = sheeps[*(_hook->target_sheep_index)];
			const glm::vec3 hook_direction = glm::normalize(_body.position - target_sheep.get_position());
			target_sheep.accelerate(hook_direction * HOOK_ACCELERATION);
		}
	}
}

void player::handle_hook(const block_container& blocks, std::vector<sheep>& sheeps) {
	if (!_hook && _actions & HOOK_ACTION) {
		_hook = hook(get_camera_position(), get_direction());
	}
	if (_hook) {
		if (_actions & HOOK_ACTION) {
			handle_active_hook(blocks, sheeps);
		} else {
			_hook.reset();
		}
	}
}
