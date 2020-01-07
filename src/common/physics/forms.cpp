#include "forms.hpp"

triangle::triangle(const glm::vec3& p1_arg, const glm::vec3& p2_arg, const glm::vec3& p3_arg)
	: p1(p1_arg), p2(p2_arg), p3(p3_arg)
{}

std::vector<triangle> triangle::split(const triangle& t) {
	glm::vec3 p12 = (t.p1 + t.p2) / 2.f;
	glm::vec3 p13 = (t.p1 + t.p3) / 2.f;
	glm::vec3 p23 = (t.p2 + t.p3) / 2.f;

	return std::vector<triangle> {
		triangle(t.p1, p12, p13),
		triangle(t.p2, p12, p23),
		triangle(t.p3, p13, p23),
		triangle(p12, p13, p23)
	};
}

void triangle::normalize() {
	p1 = glm::normalize(p1) * 0.5f;
	p2 = glm::normalize(p2) * 0.5f;
	p3 = glm::normalize(p3) * 0.5f;
}

/**
 * Returns the number of floats used for one triangle, if loaded.
 */
size_t triangle::get_size() {
	return 9;
}

void triangle::load_into(float* vertices) {
	float verts[] = {p1.x, p1.y, p1.z,
					 p2.x, p2.y, p2.z,
					 p3.x, p3.y, p3.z};
	memcpy(vertices, verts, sizeof(verts));
}

void triangle::write_to(std::vector<float>* buffer) {
	float verts[] = {p1.x, p1.y, p1.z,
					 p2.x, p2.y, p2.z,
					 p3.x, p3.y, p3.z};
	buffer->insert(buffer->end(), verts, verts + 9);
}

const glm::vec3* triangle::begin() const {
	return &p1;
}

const glm::vec3* triangle::end() const {
	return (&p3) + 1;
}

glm::vec3* triangle::begin() {
	return &p1;
}

glm::vec3* triangle::end() {
	return (&p3) + 1;
}

cuboid::cuboid() {}
cuboid::cuboid(const glm::vec3 center, const glm::vec3 size) : center(center), size(size) {}

float cuboid::get_min_x() const { return center.x - size.x; }
float cuboid::get_max_x() const { return center.x + size.x; }
float cuboid::get_min_y() const { return center.y - size.y; }
float cuboid::get_max_y() const { return center.y + size.y; }
float cuboid::get_min_z() const { return center.z - size.z; }
float cuboid::get_max_z() const { return center.z + size.z; }

ray::ray() {}
ray::ray(const glm::vec3& p, const glm::vec3& d) : position(p), direction(d) {}
