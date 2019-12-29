#include "shape_loader.hpp"

#include <iostream>
#include <glm/vec3.hpp>

#include "../../../common/world/block_container.hpp"
#include "shape_initializer.hpp"

shape_loader::shape_loader() : _run_thread(), _running(false) {}

void shape_loader::run() {
	_running = true;
	_run_thread = boost::thread(std::ref(*this));
}

void shape_loader::stop() {
	if (_running) {
		_running = false;
		_request_queue.push(chunk_request());
		_run_thread->join();
	}
}

void shape_loader::operator()() {
	while (_running) {
		load_next_chunk();
	}
}

void shape_loader::load_chunk(const block_chunk& chunk) {
	const chunk_request cr(chunk.get_block_types(), chunk.get_origin());
	_request_queue.push(cr);
}

netsi::blocking_queue<render_chunk>& shape_loader::get_update_queue() {
	return _shape_queue;
}

void shape_loader::load_next_chunk() {
	const chunk_request cr = _request_queue.pop();
	std::vector<float> vertices;

	unsigned int num_vertices = 0;

	unsigned int counter = 0;
	glm::uvec3 pos;
	for (pos.x = 0; pos.x < BLOCK_CHUNK_SIZE; pos.x++) {
		for (pos.y = 0; pos.y < BLOCK_CHUNK_SIZE; pos.y++) {
			for (pos.z = 0; pos.z < BLOCK_CHUNK_SIZE; pos.z++) {
				if (cr.blocks[counter] != block_type::VOID) {
					add_block(pos, cr.origin, &vertices);
					num_vertices += 36;
				}
				counter++;
			}
		}
	}

	std::vector<attribute> attributes = {shape::position_attribute};
	std::cout << "uploading " << num_vertices << " vertices" << std::endl;
	shape s = shape::create(vertices.data(), num_vertices, attributes, cube_specification());

	render_chunk cu(s, cr.origin);

	_shape_queue.push(cu);
}

void shape_loader::add_block(const glm::uvec3& position, const glm::ivec3& origin, std::vector<float>* vertices) {
	unsigned int coord_index = 0;
	for (float v : initialize::cube_vertices) {
		vertices->push_back(v + position[coord_index] + origin[coord_index]);
		coord_index = (coord_index+1) % 3;
	}
}
