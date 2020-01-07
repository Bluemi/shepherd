#include "shape_initializer.hpp"

#include <iostream>
#include <cstring>
#include <glm/glm.hpp>

#include "shape.hpp"

namespace initialize
{
	shape cube()
	{
		std::vector<attribute> attributes = {shape::position_attribute};

		return shape::create(cube_vertices, 36, attributes, cube_specification());
	}

	std::vector<triangle> split_triangles(std::vector<triangle> triangles, unsigned int fineness) {
		for (unsigned int i = 0; i < fineness; i++)
		{
			std::vector<triangle> tmp = triangles;
			triangles.clear();
			for (const triangle& t : tmp)
			{
				std::vector<triangle> temp = triangle::split(t);
				triangles.insert(triangles.end(), temp.begin(), temp.end());
			}
		}

		return triangles;
	}

	void normalize_triangles(std::vector<triangle>* triangles) {
		for (triangle& t : *triangles) {
			t.normalize();
		}
	}

	std::vector<triangle> create_sphere_triangles(unsigned int fineness) {
		glm::vec3 top(0.0f,  1.0f,  0.0f);
		glm::vec3 bot(0.0f, -1.0f,  0.0f);

		glm::vec3 p1( 1.0f,  0.0f,  0.0f);
		glm::vec3 p2( 0.0f,  0.0f,  1.0f);
		glm::vec3 p3(-1.0f,  0.0f,  0.0f);
		glm::vec3 p4( 0.0f,  0.0f, -1.0f);

		std::vector<triangle> triangles {
			triangle(top, p1, p2),
			triangle(top, p2, p3),
			triangle(top, p3, p4),
			triangle(top, p4, p1),

			triangle(bot, p1, p2),
			triangle(bot, p2, p3),
			triangle(bot, p3, p4),
			triangle(bot, p4, p1),
		};

		triangles = split_triangles(triangles, fineness);
		normalize_triangles(&triangles);

		return triangles;
	}

	shape sphere(unsigned int fineness) {
		std::vector<triangle> triangles = create_sphere_triangles(fineness);

		unsigned int number_floats = triangle::get_size();

		float vertices[number_floats*triangles.size()];
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			triangles[i].load_into(vertices+i*number_floats);
		}

		std::vector<attribute> attributes = {shape::position_attribute};

		return shape::create(vertices, triangles.size() * 3, attributes, sphere_specification(fineness));
	}

	void add_point(const glm::vec3& p, std::vector<float>* vertices) {
		vertices->push_back(p.x);
		vertices->push_back(p.y);
		vertices->push_back(p.z);
	}

	shape sheep() {
		std::vector<float> sheep_vertices;

		// body
		const glm::vec3 body_color = glm::vec3(0.6f);
		const glm::vec3 body_scale = glm::vec3(0.8f, 0.65f, 0.7f);
		const glm::vec3 body_translate = glm::vec3(-0.05f, 0.f, 0.f);

		std::vector<triangle> body_triangles = create_sphere_triangles(2);
		for (const triangle& t : body_triangles) {
			add_point(t.p1*body_scale + body_translate, &sheep_vertices); // add position
			add_point(body_color, &sheep_vertices); // add color

			add_point(t.p2*body_scale + body_translate, &sheep_vertices); // add position
			add_point(body_color, &sheep_vertices); // add color

			add_point(t.p3*body_scale + body_translate, &sheep_vertices); // add position
			add_point(body_color, &sheep_vertices); // add color
		}

		// head
		const glm::vec3 head_color(0.05f);
		const glm::vec3 head_scale(0.25f, 0.32f, 0.25f);
		const glm::vec3 head_translate(0.37f, 0.1f, 0.f);

		std::vector<triangle> head_triangles = create_sphere_triangles(2);
		for (const triangle& t : head_triangles) {
			add_point(t.p1*head_scale + head_translate, &sheep_vertices); // add position
			add_point(head_color, &sheep_vertices); // add color

			add_point(t.p2*head_scale + head_translate, &sheep_vertices); // add position
			add_point(head_color, &sheep_vertices); // add color

			add_point(t.p3*head_scale + head_translate, &sheep_vertices); // add position
			add_point(head_color, &sheep_vertices); // add color
		}

		// eyes
		const glm::vec3 eye_color(0.4f, 0.4f, 0.5f);
		const glm::vec3 eye_scale(0.07f);
		const float eye_x_offset = 0.44f;
		const float eye_z_offset = 0.06f;
		const std::vector<glm::vec3> eye_translates{
			{eye_x_offset, 0.1f,  eye_z_offset},
			{eye_x_offset, 0.1f, -eye_z_offset}
		};

		for (const glm::vec3& eye_translate : eye_translates) {
			for (const triangle& t : head_triangles) {
				add_point(t.p1 * eye_scale + eye_translate, &sheep_vertices); // add position
				add_point(eye_color, &sheep_vertices); // add color

				add_point(t.p2 * eye_scale + eye_translate, &sheep_vertices); // add position
				add_point(eye_color, &sheep_vertices); // add color

				add_point(t.p3 * eye_scale + eye_translate, &sheep_vertices); // add position
				add_point(eye_color, &sheep_vertices); // add color
			}
		}

		// legs
		const glm::vec3 leg_color = glm::vec3(0.05f);
		const glm::vec3 leg_scale(0.07f, 0.3f, 0.07f);
		const float leg_x_offset = 0.2f;
		const float leg_y_offset = -0.35f;
		const float leg_z_offset = 0.15f;
		const std::vector<glm::vec3> leg_translates {
			{ leg_x_offset-0.05f, leg_y_offset, -leg_z_offset}, // left front
			{ leg_x_offset-0.05f, leg_y_offset,  leg_z_offset}, // right front
			{-leg_x_offset-0.05f, leg_y_offset, -leg_z_offset}, // left back
			{-leg_x_offset-0.05f, leg_y_offset,  leg_z_offset} // right back
		};

		for (const glm::vec3& leg_translate : leg_translates) {
			for (const triangle& t : cube_triangles) {
				add_point(t.p1 * leg_scale + leg_translate, &sheep_vertices); // add position
				add_point(leg_color, &sheep_vertices); // add color

				add_point(t.p2 * leg_scale + leg_translate, &sheep_vertices); // add position
				add_point(leg_color, &sheep_vertices); // add color

				add_point(t.p3 * leg_scale + leg_translate, &sheep_vertices); // add position
				add_point(leg_color, &sheep_vertices); // add color
			}
		}

		std::vector<attribute> attributes = {shape::position_attribute, shape::color_attribute};

		return shape::create(sheep_vertices.data(), sheep_vertices.size() / 6, attributes, sheep_specification());
	}
}
