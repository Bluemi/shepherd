#include "block_container.hpp"

#include <iostream>

#include <glm/gtc/noise.hpp>
#include <glm/gtx/norm.hpp>

#include "../physics/forms.hpp"
#include "../physics/util.hpp"

constexpr unsigned int MAP_X_SIZE = 128;
constexpr unsigned int MAP_Z_SIZE = 32;
constexpr float WINNING_COLOR_WHITE = 0.3f;
constexpr float WINNING_COLOR_BLACK = 0.03f;
constexpr float NOISE_SCALE = 0.05f;
constexpr float MAP_HEIGHT = 5.f;

block_chunk::block_chunk()
	: _block_types(BLOCK_CHUNK_SIZE*BLOCK_CHUNK_SIZE*BLOCK_CHUNK_SIZE, block_type::VOID)
{}

block_chunk::block_chunk(const glm::ivec3& origin)
	: _block_types(BLOCK_CHUNK_SIZE*BLOCK_CHUNK_SIZE*BLOCK_CHUNK_SIZE, block_type::VOID),
	  _origin(origin)
{}

block_type block_chunk::get_block_type(const glm::ivec3& position) const {
	return get_local_block_type(global_to_local(position));
}

block_type block_chunk::get_local_block_type(const glm::uvec3& position) const {
	return _block_types[block_chunk::get_index(position)];
}

const std::vector<block_type>& block_chunk::get_block_types() const {
	return _block_types;
}

const glm::ivec3& block_chunk::get_origin() const {
	return _origin;
}

glm::ivec3 block_chunk::get_top() const {
	return _origin + glm::ivec3(BLOCK_CHUNK_SIZE);
}

bool block_chunk::contains(const glm::ivec3& position) const {
	return glm::all(glm::greaterThanEqual(position, _origin)) && glm::all(glm::lessThan(position, get_top()));
}

void block_chunk::set_block_type(const glm::ivec3& position, block_type bt) {
	_block_types[get_index(global_to_local(position))] = bt;
}

unsigned int block_chunk::get_index(const glm::uvec3& position) {
	return position.x*BLOCK_CHUNK_SIZE*BLOCK_CHUNK_SIZE + position.y*BLOCK_CHUNK_SIZE + position.z;
}

glm::uvec3 block_chunk::global_to_local(const glm::ivec3& position) const {
	return position - _origin;
}

// ------- BLOCK CONTAINER -------

block_container::block_container() : _min_y(0) {}
block_container::block_container(const std::vector<world_block>& blocks) : _block_chunks(), _min_y(0) {
	for (const world_block& b : blocks) {
		add_block(b.get_position(), b.get_type());
	}
}

int get_height(int x, int z, unsigned seed) {
	int z_sym = std::abs(z - (int)MAP_Z_SIZE/2);
	float h_f = glm::perlin(glm::vec2((x+seed)*NOISE_SCALE,      (z_sym+seed)*NOISE_SCALE)) + 
				glm::perlin(glm::vec2((x+seed)*NOISE_SCALE*3.0,  (z_sym+seed)*NOISE_SCALE*3.0))*0.4f +
				glm::perlin(glm::vec2((x+seed)*NOISE_SCALE*0.2,  (z_sym+seed)*NOISE_SCALE*0.2))*2.f +
				glm::perlin(glm::vec2((x+seed)*NOISE_SCALE*0.02, (z_sym+seed)*NOISE_SCALE*0.02))*10.f;

	return glm::floor(h_f*MAP_HEIGHT / (1.f + glm::exp(0.25f*(static_cast<float>(x)-MAP_X_SIZE)+3.f)));
}

std::vector<world_block> block_container::create_field(unsigned int seed) {
	std::vector<world_block> blocks;

	unsigned int s = seed % 25000;

	// initialize blocks
	for (unsigned int x = 0; x < MAP_X_SIZE; x++) {
		for (unsigned int z = 0; z < MAP_Z_SIZE; z++) {
			int h = get_height(x, z, s);

			for (int y = h-3; y < h; y++) {
				blocks.push_back(world_block(glm::ivec3(x, y, z), block_type::NORMAL));
			}
		}
	}

	return blocks;
}

int norm(int x) {
	return BLOCK_CHUNK_SIZE * glm::floor(static_cast<float>(x) / static_cast<float>(BLOCK_CHUNK_SIZE));
}

glm::ivec3 block_container::to_chunk_position(const glm::ivec3& position) {
	return glm::ivec3(
		norm(position.x),
		norm(position.y),
		norm(position.z)
	);
}

glm::vec3 block_container::get_respawn_position() const {
	int x = (rand() % 5)+1;
	int y = 10.f;
	int z = rand() % MAP_Z_SIZE;

	for (int yi = -200; yi < 200; yi++) {
		if (get_block(glm::ivec3(x, yi, z))) {
			y = yi;
		}
	}

	return glm::vec3(x, y+3.f, z);
}

std::optional<world_block> block_container::get_block(const glm::ivec3& position) const {
	const block_chunk* bc = get_containing_chunk(position);
	if (bc) {
		block_type bt = bc->get_block_type(position);
		if (bt != block_type::VOID) {
			return world_block(position, bt);
		}
	}
	return {};
}

const std::vector<std::shared_ptr<block_chunk>>& block_container::get_chunks() const {
	return _block_chunks;
}

const block_chunk* block_container::get_containing_chunk(const glm::ivec3& position) const {
	for (const std::shared_ptr<block_chunk>& bc : _block_chunks) {
		if (bc->contains(position)) {
			return &(*bc);
		}
	}
	return nullptr;
}

block_chunk* block_container::get_containing_chunk(const glm::ivec3& position) {
	return const_cast<block_chunk*>(const_cast<const block_container*>(this)->get_containing_chunk(position));
}

void block_container::add_block(const glm::ivec3& position, block_type bt) {
	block_chunk* bc = get_containing_chunk(position);
	if (bc == nullptr) {
		bc = add_chunk(position);
	}
	bc->set_block_type(position, bt);

	if (position.y < _min_y) {
		_min_y = position.y;
	}
}

block_chunk* block_container::add_chunk(const glm::ivec3& position) {
	glm::ivec3 chunk_origin = to_chunk_position(position);
	std::shared_ptr<block_chunk> chunk = std::make_shared<block_chunk>(chunk_origin);
	_block_chunks.push_back(chunk);
	return &(*chunk);
}

bool block_container::remove_block(const glm::ivec3& position) {
	block_chunk* bc = get_containing_chunk(position);
	if (bc == nullptr) {
		return false;
	}

	bc->set_block_type(position, block_type::VOID);
	return true;
}

std::vector<world_block> block_container::get_colliding_blocks(const cuboid& box) const {
	std::vector<world_block> colliding_blocks;

	int min_x_index = static_cast<int>(glm::floor(box.get_min_x() + 0.5f));
	int max_x_index = static_cast<int>(glm::floor(box.get_max_x() + 0.5f));
	int min_y_index = static_cast<int>(glm::floor(box.get_min_y() + 0.5f));
	int max_y_index = static_cast<int>(glm::floor(box.get_max_y() + 0.5f));
	int min_z_index = static_cast<int>(glm::floor(box.get_min_z() + 0.5f));
	int max_z_index = static_cast<int>(glm::floor(box.get_max_z() + 0.5f));

	for (int x = min_x_index; x <= max_x_index; x++) {
		for (int y = min_y_index; y <= max_y_index; y++) {
			for (int z = min_z_index; z <= max_z_index; z++) {
				std::optional<world_block> b = get_block(glm::ivec3(x, y, z));
				if (b) {
					world_block wb = *b;
					if (
						wb.get_position().x - 0.5f < box.get_max_x() && wb.get_position().x + 0.5f > box.get_min_x() &&
						wb.get_position().y - 0.5f < box.get_max_y() && wb.get_position().y + 0.5f > box.get_min_y() &&
						wb.get_position().z - 0.5f < box.get_max_z() && wb.get_position().z + 0.5f > box.get_min_z()
					) {
						colliding_blocks.push_back(wb);
					}
				}
			}
		}
	}

	return colliding_blocks;
}

unsigned int argmin(const glm::vec3& v) {
	float min_value = v.x;
	unsigned int min_index = 0;
	for (unsigned int i = 1; i < v.length(); i++) {
		const float value = v[i];
		if (value < min_value) {
			min_value = value;
			min_index = i;
		}
	}
	return min_index;
}

std::optional<world_block> block_container::get_colliding_block(const ray& r, float max_range) const {
	glm::vec3 current_position = r.position;
	glm::ivec3 current_block = glm::round(r.position);

	const glm::vec3 next_block_direction = glm::sign(r.direction);
	const float max_range2 = max_range*max_range;

	while (glm::distance2(current_position, r.position) < max_range2) {
		std::optional<world_block> cb = get_block(current_block);
		if (cb) return cb;

		glm::vec3 next_block = glm::vec3(current_block) + next_block_direction;
		const glm::vec3 distances = current_position - (next_block - next_block_direction*0.5f);
		const glm::vec3 rel_distances = glm::abs(distances / r.direction);
		unsigned int min_index = argmin(rel_distances);
		current_block[min_index] += next_block_direction[min_index];
		current_position += r.direction * rel_distances[min_index];
	}

	return {};
}

std::optional<glm::ivec3> block_container::get_addition_position(const ray& r, float max_range) const {
	glm::vec3 current_position = r.position;
	glm::ivec3 current_block = glm::round(r.position);
	glm::ivec3 last_block = current_block;

	const glm::vec3 next_block_direction = glm::sign(r.direction);
	const float max_range2 = max_range*max_range;

	while (glm::distance2(current_position, r.position) < max_range2) {
		std::optional<world_block> cb = get_block(current_block);
		if (cb) return last_block;

		const glm::vec3 next_block = glm::vec3(current_block) + next_block_direction;
		const glm::vec3 distances = current_position - (next_block - next_block_direction*0.5f);
		const glm::vec3 rel_distances = glm::abs(distances / r.direction);
		const unsigned int min_index = argmin(rel_distances);
		last_block = current_block;
		current_block[min_index] += next_block_direction[min_index];
		current_position += r.direction * rel_distances[min_index];
	}

	return {};
}

glm::vec3 block_container::get_color(const glm::ivec3& position) {
	const float blue  = glm::perlin(glm::vec2(position.x*0.1f        , position.z*0.1f + 100.f ))*0.02f + 0.03f;
	const float red   = glm::perlin(glm::vec2(position.x*0.1f + 200.f, position.z*0.1f + 300.f ))*0.02f + 0.1f - glm::max(blue, 0.f)*0.6f;
	const float green = glm::perlin(glm::vec2(position.x*0.1f + 400.f, position.z*0.1f + 500.f ))*0.03f + 0.12f - glm::max(blue, 0.f)*0.3f;
	return glm::vec3(red, green, blue);
}

glm::vec3 block_container::get_winning_color(const glm::ivec3& position) {
	return glm::vec3(
		(position.x+position.y+position.z)%2?WINNING_COLOR_WHITE:WINNING_COLOR_BLACK,
		(position.x+position.y+position.z)%2?WINNING_COLOR_WHITE:WINNING_COLOR_BLACK,
		(position.x+position.y+position.z)%2?WINNING_COLOR_WHITE:WINNING_COLOR_BLACK
	);
}

int block_container::get_min_y() const {
	return _min_y;
}
