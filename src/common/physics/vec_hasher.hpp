#ifndef __VEC_HASHER_CLASS__
#define __VEC_HASHER_CLASS__

#include <glm/vec3.hpp>

struct vec_hasher {
	std::size_t operator()(const glm::ivec3& v) const;
};

#endif
