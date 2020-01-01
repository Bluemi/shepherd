#include "shape_loader.hpp"

#include <glm/vec3.hpp>

#include "../../../common/world/block_container.hpp"
#include "shape_initializer.hpp"

render_chunk do_load_chunk(const chunk_request& cr) {
	unsigned int num_blocks = 0;
	for (block_type bt : cr.blocks) {
		if (bt != block_type::VOID) {
			num_blocks++;
		}
	}

	std::vector<float> vertices;
	vertices.reserve(num_blocks*6*36);

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

	std::vector<attribute> attributes = {shape::position_attribute, shape::color_attribute};
	shape s = shape::create(vertices.data(), num_vertices, attributes, cube_specification());

	return render_chunk(s, cr.origin);
}

void add_block(const glm::uvec3& position, const glm::ivec3& origin, std::vector<float>* vertices) {
	unsigned int coord_index = 0;
	glm::vec3 vert;
	for (float v : initialize::cube_vertices) {
		vertices->push_back(v + position[coord_index]);
		vert[coord_index] = v;
		coord_index = (coord_index+1) % 3;

		// add color
		if (coord_index == 0) {
			glm::vec3 color(block_container::get_color(glm::ivec3(position) + origin));
			vertices->push_back(color.r + (vert.y*0.05 + vert.z*0.015 + vert.x*0.012));
			vertices->push_back(color.g + (vert.y*0.05 + vert.z*0.013 + vert.x*0.017));
			vertices->push_back(color.b + (vert.y*0.05 + vert.z*0.011 + vert.x*0.019));
		}
	}
}
