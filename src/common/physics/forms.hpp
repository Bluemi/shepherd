#ifndef __FORMS_CLASS__
#define __FORMS_CLASS__

#include <vector>
#include <cstring>
#include <glm/glm.hpp>

class triangle {
	public:
		triangle(const glm::vec3& p1_arg, const glm::vec3& p2_arg, const glm::vec3& p3_arg);

		const static unsigned char POSITION_BIT = 0b00000001;
		const static unsigned char NORMALE_BIT  = 0b00000010;

		static std::vector<triangle> split(const triangle& t);
		void normalize();

		/**
		 * Returns the number of floats used for one triangle, if loaded.
		 */
		static size_t get_size();

		void load_into(float* vertices);
		void write_to(std::vector<float>* buffer);

		const glm::vec3* begin() const;
		const glm::vec3* end() const;

		glm::vec3* begin();
		glm::vec3* end();

		glm::vec3 p1, p2, p3;
};


struct cuboid {
	cuboid();
	cuboid(const glm::vec3 center, const glm::vec3 size);

	float get_min_x() const;
	float get_max_x() const;
	float get_min_y() const;
	float get_max_y() const;
	float get_min_z() const;
	float get_max_z() const;

	glm::vec3 center;
	glm::vec3 size;
};

struct ray {
	ray();
	ray(const glm::vec3& p, const glm::vec3& d);

	glm::vec3 get_tip() const {
		return position + direction;
	}

	glm::vec3 position;
	glm::vec3 direction;
};

struct sphere {
	glm::vec3 position;
	float radius;
};

#endif
