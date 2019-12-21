#ifndef __SHAPEINITIALIZER_CLASS__
#define __SHAPEINITIALIZER_CLASS__

#include <map>
#include <variant>

#include "shape.hpp"

namespace initialize
{
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
}

#endif
