#ifndef __SHAPE_LOADER_CLASS__
#define __SHAPE_LOADER_CLASS__

#include <vector>
#include <glm/vec3.hpp>

#include "shape.hpp"
#include "../../../common/world/world_block.hpp"

class block_chunk;

struct render_chunk {
	render_chunk(const shape& s, const glm::ivec3& o) : chunk_shape(s), origin(o) {}

	shape chunk_shape;
	glm::ivec3 origin;
};

struct chunk_request {
	chunk_request() {}
	chunk_request(const std::vector<block_type>& b, const glm::ivec3& o) : blocks(b), origin(o) {}

	std::vector<block_type> blocks;
	glm::ivec3 origin;
};

render_chunk do_load_chunk(const chunk_request& cr);
void add_block(const glm::uvec3& position, const glm::ivec3& origin, std::vector<float>* vertices);

#endif
