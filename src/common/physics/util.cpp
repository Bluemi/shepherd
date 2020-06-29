#include "util.hpp"

#include <glm/gtc/type_ptr.hpp>

std::ostream& operator<<(std::ostream& stream, const glm::vec3& v) {
	stream << '(' << v.x << ',' << v.y << ',' << v.z << ')';
	return stream;
}

glm::mat4 basis_perspective(float fovy, float aspect_ratio, float near, float far, const glm::vec3& basis) {
	float tangent = tan(fovy/2.f);   // tangent of half fovY
    float height = near * tangent;          // half height of near plane
    float width = height * aspect_ratio;      // half width of near plane

	float perspective_data[16] = {
		// x row
		(near+basis.z)/width, 0.f,                   basis.x,                0.f,
		// y row
		0.f,                  (near+basis.z)/height, basis.y,                0.f,
		// z row
		0.f,                  0.f,                   -(far+near)/(far-near), -2.f*(far*near)/(far-near),
		// w row
		0.f,                  0.f,                   -1.f,                   0.f,
	};
	glm::mat4 perspective = glm::transpose(glm::make_mat4(perspective_data));
	return perspective;
}
