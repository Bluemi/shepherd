#ifndef __FORMS_CLASS__
#define __FORMS_CLASS__

#include <glm/glm.hpp>

struct cuboid {
	cuboid() {}
	cuboid(const glm::vec3 center, const glm::vec3 size) : center(center), size(size) {}

	float get_min_x() const { return center.x - size.x; }
	float get_max_x() const { return center.x + size.x; }
	float get_min_y() const { return center.y - size.y; }
	float get_max_y() const { return center.y + size.y; }
	float get_min_z() const { return center.z - size.z; }
	float get_max_z() const { return center.z + size.z; }

	glm::vec3 center;
	glm::vec3 size;
};

struct sphere {
	glm::vec3 position;
	float radius;
};

#endif
