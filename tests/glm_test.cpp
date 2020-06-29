#include <iostream>
#include <glm/vec3.hpp>
#include <glm/gtx/projection.hpp>

std::string str(const glm::vec3& vec) {
	return "(" + std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + ")";
}

int main() {
	glm::vec3 p(0, 0, 0);
	glm::vec3 r(0, 1, 0);
	glm::vec3 d(1, 1, 1);

	glm::vec3 result = glm::proj(d, r);

	std::cout << str(result) << std::endl;
	return 0;
}
