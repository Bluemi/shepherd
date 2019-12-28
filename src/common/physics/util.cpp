#include "util.hpp"

std::ostream& operator<<(std::ostream& stream, const glm::vec3& v) {
	stream << '(' << v.x << ',' << v.y << ',' << v.z << ')';
	return stream;
}
