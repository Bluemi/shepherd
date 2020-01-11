#ifndef __SHAPEINITIALIZER_CLASS__
#define __SHAPEINITIALIZER_CLASS__

#include "../../../common/physics/forms.hpp"
#include "shape.hpp"

namespace initialize {
	const static float cube_vertices[] {
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

	const static triangle cube_triangles[] {
		// min z
		triangle(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.5f,  0.5f, -0.5f)),
		triangle(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f)),

		// max z
		triangle(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.5f,  0.5f,  0.5f)),
		triangle(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-0.5f, -0.5f,  0.5f)),

		// min x
		triangle(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f)),
		triangle(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f)),

		// max x
		triangle(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.5f, -0.5f, -0.5f)),
		triangle(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.5f,  0.5f,  0.5f)),

		// min y
		triangle(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.5f, -0.5f,  0.5f)),
		triangle(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-0.5f, -0.5f, -0.5f)),

		// max y
		triangle(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.5f,  0.5f,  0.5f)),
		triangle(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f, -0.5f))
	};

	constexpr float VISOR_SMALL_SIZE = 1.0f;
	constexpr float VISOR_BIG_SIZE = 0.03f;

	const static float visor_vertices[] {
		// —
		-VISOR_BIG_SIZE, -VISOR_SMALL_SIZE, // bottom
		 VISOR_BIG_SIZE, -VISOR_SMALL_SIZE,
		-VISOR_BIG_SIZE,  VISOR_SMALL_SIZE,

		-VISOR_BIG_SIZE,  VISOR_SMALL_SIZE, // top
		 VISOR_BIG_SIZE,  VISOR_SMALL_SIZE,
		 VISOR_BIG_SIZE, -VISOR_SMALL_SIZE,

		// |
		-VISOR_SMALL_SIZE, -VISOR_BIG_SIZE, // left
		-VISOR_SMALL_SIZE,  VISOR_BIG_SIZE,
		 VISOR_SMALL_SIZE,  VISOR_BIG_SIZE,

		 VISOR_SMALL_SIZE,  VISOR_BIG_SIZE, // right
		 VISOR_SMALL_SIZE, -VISOR_BIG_SIZE,
		-VISOR_SMALL_SIZE, -VISOR_BIG_SIZE
	};

	struct triangle_coordinate_index {
		triangle_coordinate_index(std::uint8_t c, std::int8_t d) : coord(c), direction(d) {}

		std::uint8_t coord;
		std::int8_t direction;
	};

	const static triangle_coordinate_index triangle_coordinate_indices[] {
		triangle_coordinate_index(2, -1), // min z
		triangle_coordinate_index(2,  1), // max z
		triangle_coordinate_index(0, -1), // min x
		triangle_coordinate_index(0,  1), // max x
		triangle_coordinate_index(1, -1), // min y
		triangle_coordinate_index(1,  1), // max y
	};

	/**
	 * Returns a Shape defining a cube.
	 */
	shape cube();

	/**
	 * Returns a Shape defining a sphere.
	 *
	 * @param fineness Defines how fine the sphere is built.
	 *
	 * 				   The number of triangles used to draw the sphere is defined by:
	 * 				   n_triangles = 8 * 4 ^ fineness
	 */
	shape sphere(unsigned int fineness);

	shape sheep();

	shape visor();
}

#endif
