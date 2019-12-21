#ifndef __SHAPE_CLASS__
#define __SHAPE_CLASS__

#include <vector>
#include <variant>

#include "shape_specification.hpp"

/**
 * Type to define number of vertices.
 */
using n_vertices = size_t;

/**
 * Type to define number of floats.
 */
using n_floats = std::size_t;

/**
 * Type to define number of triangles.
 */
using n_triangles = std::size_t;

/**
 * An attribute defining the size in number of floats and the type of vertices.
 */
struct attribute
{
	attribute(n_floats s, unsigned int t)
		: size(s), type(t)
	{}
	/**
	 * The number of floats this attribute takes.
	 */
	n_floats size;

	/**
	 * The type of this attribute
	 */
	unsigned int type;
};

/**
 * A shape defines the following attributes:
 *   - Vertices
 *   - Indices if any
 *   - Attributepointers
 *   - An shape_specification
 */
class shape {
	public:
		shape() {}

		/**
		 * Deletes all used buffers of this shape.
		 * After calling this function this shape cant be used for rendering.
		 */
		void free_buffers();

		/**
		 * Creates a new shape.
		 * This shape is bound.
		 *
		 * @param vertices The vertices of the shape
		 * @param vertices_size The number of the vertices.
		 * 						If you have 2 triangles this would be 2*3=6
		 * @param attributes A list of attributes, which define the order of the vertices.
		 */
		static shape create(const float* vertices,
							n_vertices number_vertices,
							const std::vector<attribute>& attributes,
							const shape_specification& specification);

		/**
		 * Binds this shape to use for rendering.
		 * After this call the Vertices, Indices and AttributePointer of this
		 * shape are used by any call of glDrawArrays or glDrawElements.
		 */
		void bind() const;

		// Getter
		bool use_indices() const;
		n_vertices get_number_vertices() const;
		shape_specification get_specification() const;

		/**
		 * Unbinds all shapes.
		 */
		static void unbind();

		/*
		 * Attributes, which can be useful.
		 */
		static const attribute position_attribute;
		static const attribute color_attribute;
		static const attribute texture_coordinate_attribute;
		static const attribute normale_attribute;

	private:
		/**
		 * Creates a new shape
		 *
		 * @param vertex_array_object The id of the vao of this shape
		 * @param vertex_buffer_object The id of the vbo of this shape
		 * @param number_vertices The number of vertices used by this shape
		 * @param use_indices If true use glDrawElements otherwise glDrawArrays.
		 */
		shape(unsigned int vertex_array_object,
			  unsigned int vertex_buffer_object,
			  n_vertices number_vertices,
			  bool use_indices,
			  const shape_specification& specification);

		/**
		 * Creates the vertex array object of this shape and binds it.
		 *
		 * @return The id of the created vao.
		 */
		static unsigned int create_vao();

		/**
		 * Buffers the given vertices and returns the id of the created vbo.
		 *
		 * @param vertices The vertices as float array
		 * @param vertices_size The size number of all vertices added together in bytes
		 * @return The id of the new vbo
		 */
		static unsigned int buffer_vertices(const float* vertices, size_t vertices_size);

		/**
		 * @return The sum of the number of floats used by all attributes.
		 */
		static n_floats get_attributes_size(const std::vector<attribute>& attributes);
		static void create_attribute_pointer(const std::vector<attribute>& attributes);

		unsigned int _vertex_array_object;
		unsigned int _vertex_buffer_object;
		n_vertices _number_vertices;
		bool _use_indices;
		shape_specification _specification;
};

#endif
