#include "shape_loader.hpp"

#include <glm/vec3.hpp>

#include "../../../common/world/block_container.hpp"
#include "shape_initializer.hpp"

render_chunk do_load_chunk(const chunk_request& cr) {
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

	std::vector<attribute> attributes = {shape::position_attribute, shape::color_attribute};
	shape s = shape::create(vertices.data(), num_vertices, attributes, cube_specification());

	return render_chunk(s, cr.origin);
}

void add_block(const glm::uvec3& position, const glm::ivec3& origin, std::vector<float>* vertices) {
	unsigned int coord_index = 0;
	for (float v : initialize::cube_vertices) {
		vertices->push_back(v + position[coord_index]);
		coord_index = (coord_index+1) % 3;

		// add color
		if (coord_index == 0) {
			glm::vec3 color(block_container::get_color(glm::ivec3(position) + origin));
			vertices->push_back(color.r);
			vertices->push_back(color.g);
			vertices->push_back(color.b);
		}
	}
}
