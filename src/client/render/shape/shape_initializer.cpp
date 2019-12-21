#include "shape_initializer.hpp"

#include <cstring>
#include <glm/glm.hpp>

#include "shape.hpp"

namespace initialize
{
	shape cube()
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		std::vector<attribute> attributes = {shape::position_attribute};

		return shape::create(vertices, 36, attributes, cube_specification());
	}

	class triangle
	{
		public:
			triangle(const glm::vec3& p1_arg, const glm::vec3& p2_arg, const glm::vec3& p3_arg)
				: p1(p1_arg), p2(p2_arg), p3(p3_arg)
			{}

			const static unsigned char POSITION_BIT = 0b00000001;
			const static unsigned char NORMALE_BIT  = 0b00000010;

			static std::vector<triangle> split(const triangle& t)
			{
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

			void normalize()
			{
				p1 = glm::normalize(p1);
				p2 = glm::normalize(p2);
				p3 = glm::normalize(p3);
			}

			/**
			 * Returns the number of floats used for one triangle, if loaded.
			 */
			static size_t get_size() {
				return 9;
			}

			void load_into(float* vertices) {
				float verts[] = {p1.x, p1.y, p1.z,
								 p2.x, p2.y, p2.z,
								 p3.x, p3.y, p3.z};
				memcpy(vertices, verts, sizeof(verts));
			}

			glm::vec3 p1, p2, p3;
	};

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
