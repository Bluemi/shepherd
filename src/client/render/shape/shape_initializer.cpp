#include "shape_initializer.hpp"

#include <cstring>
#include <glm/glm.hpp>

#include "shape.hpp"
#include "../../../common/physics/forms.hpp"

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

	shape sphere(unsigned int fineness)
	{
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

		unsigned int number_floats = triangle::get_size();

		float vertices[number_floats*triangles.size()];
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			triangles[i].load_into(vertices+i*number_floats);
		}

		std::vector<attribute> attributes = {shape::position_attribute};

		return shape::create(vertices, triangles.size() * 3, attributes, sphere_specification(fineness));
	}
}
