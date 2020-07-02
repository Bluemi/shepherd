#include "util.hpp"

#include <glm/gtc/type_ptr.hpp>

std::ostream& operator<<(std::ostream& stream, const glm::vec3& v) {
	stream << '(' << v.x << ',' << v.y << ',' << v.z << ')';
	return stream;
}

glm::mat4 basis_perspective(float screen_width, float screen_height, float near, float far, const glm::vec3& screen_position) {
	float left = (screen_position.x - (screen_width / 2.f)) / screen_position.z * near;
	float right = (screen_position.x + (screen_width / 2.f)) / screen_position.z * near;
	float top = (screen_position.y + (screen_height / 2.f)) / screen_position.z * near;
	float bottom = (screen_position.y - (screen_height / 2.f)) / screen_position.z * near;
	return frustum_perspective(near, far, left, right, top, bottom);
}

/*
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
*/

glm::mat4 frustum_perspective(float near, float far, float left, float right, float top, float bottom) {
	float perspective_data[16] = {
		// x row
		2.f*near/(right-left), 0.f,                   (right+left)/(right-left),                0.f,
		// y row
		0.f,                  (2.f*near)/(top-bottom), (top+bottom)/(top-bottom),                0.f,
		// z row
		0.f,                  0.f,                   -(far+near)/(far-near), (-2.f*far*near)/(far-near),
		// w row
		0.f,                  0.f,                   -1.f,                   0.f,
	};
	glm::mat4 perspective = glm::transpose(glm::make_mat4(perspective_data));
	return perspective;
}
