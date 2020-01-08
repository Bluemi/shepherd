#include "shape_loader.hpp"

#include <iostream>
#include <glm/vec3.hpp>

#include "../../../common/world/block_container.hpp"
#include "shape_initializer.hpp"
#include "shape_initializer.hpp"

render_chunk do_load_chunk(const chunk_request& cr) {
	std::vector<float> vertices;

	unsigned int counter = 0;
	glm::uvec3 pos;

	for (pos.x = 0; pos.x < BLOCK_CHUNK_SIZE; pos.x++) {
		for (pos.y = 0; pos.y < BLOCK_CHUNK_SIZE; pos.y++) {
			for (pos.z = 0; pos.z < BLOCK_CHUNK_SIZE; pos.z++) {
				if (cr.blocks[counter] != block_type::VOID) {
					add_block(pos, cr.origin, &vertices, cr.blocks[counter], cr.blocks);
				}
				counter++;
			}
		}
	}

	std::vector<attribute> attributes = {shape::position_attribute, shape::color_attribute};
	shape s = shape::create(vertices.data(), vertices.size()/6, attributes);

	return render_chunk(s, cr.origin);
}

bool triangle_visible(const glm::uvec3& position, const initialize::triangle_coordinate_index& tci, const std::vector<block_type>& blocks) {
	glm::ivec3 neighbor_position = position;
	neighbor_position[tci.coord] += tci.direction;

	// if border of chunk, always visible
	if (neighbor_position[tci.coord] < 0 || neighbor_position[tci.coord] >= (int)BLOCK_CHUNK_SIZE) {
		return true;
	}

	// only visible if neighbor is void block
	return blocks[neighbor_position.x*BLOCK_CHUNK_SIZE*BLOCK_CHUNK_SIZE + neighbor_position.y*BLOCK_CHUNK_SIZE + neighbor_position.z] == block_type::VOID;
}

void add_point(const glm::vec3& p, const glm::ivec3& position, std::vector<float>* vertices, const glm::ivec3& origin, block_type bt) {
	// add coordinate
	vertices->push_back(p.x + position.x);
	vertices->push_back(p.y + position.y);
	vertices->push_back(p.z + position.z);

	// add color
	glm::vec3 color;
	switch (bt) {
		case block_type::GROUND:
			color = glm::vec3(0.02f, 0.02f, 0.02f);
			break;
		case block_type::NORMAL:
			color = block_container::get_color(position + origin);
			break;
		case block_type::WINNING:
			color = block_container::get_winning_color(position + origin);
			break;
		default:
			std::cerr << "shape_loader::add_block(): cant indentify block type" << std::endl;
			break;
	}
	vertices->push_back(color.r + (p.y*0.05 + p.z*0.015 + p.x*0.012));
	vertices->push_back(color.g + (p.y*0.05 + p.z*0.013 + p.x*0.017));
	vertices->push_back(color.b + (p.y*0.05 + p.z*0.011 + p.x*0.019));
}

void add_triangle(const glm::uvec3& position, const triangle& t, std::vector<float>* vertices, const glm::ivec3& origin, block_type bt) {
	add_point(t.p1, position, vertices, origin, bt);
	add_point(t.p2, position, vertices, origin, bt);
	add_point(t.p3, position, vertices, origin, bt);
}

void add_block(const glm::uvec3& position, const glm::ivec3& origin, std::vector<float>* vertices, block_type bt, const std::vector<block_type>& blocks) {
	unsigned int index = 0;
	for (const triangle& cube_triangle : initialize::cube_triangles) {
		if (triangle_visible(position, initialize::triangle_coordinate_indices[index/2], blocks)) {
			add_triangle(position, cube_triangle, vertices, origin, bt);
		}

		index++;
	}
}
