#include "shape.hpp"

#include <glad/glad.h>

#include "shape_specification.hpp"

const attribute shape::position_attribute(3, GL_FLOAT);
const attribute shape::color_attribute(3, GL_FLOAT);
const attribute shape::texture_coordinate_attribute(2, GL_FLOAT);
const attribute shape::normale_attribute(3, GL_FLOAT);

shape::shape(unsigned int vertex_array_object,
			 unsigned int vertex_buffer_object,
			 n_vertices number_vertices,
			 bool use_indices,
			 const shape_specification& specification)
	: _vertex_array_object(vertex_array_object),
	  _vertex_buffer_object(vertex_buffer_object),
	  _number_vertices(number_vertices),
	  _use_indices(use_indices),
	  _specification(specification)
{}

void shape::free_buffers() {
	glDeleteVertexArrays(1, &_vertex_array_object);
}

shape shape::create(
	const float* vertices,
	n_vertices number_vertices,
	const std::vector<attribute>& attributes,
	const shape_specification& specification
) {
	unsigned int vao = create_vao();
	unsigned int attributes_size = get_attributes_size(attributes);
	unsigned int vbo;
	vbo = buffer_vertices(vertices, number_vertices * attributes_size * sizeof(float));
	create_attribute_pointer(attributes);

	return shape(vao, vbo, number_vertices, false, specification);
}

void shape::bind() const {
	glBindVertexArray(_vertex_array_object);
}

bool shape::use_indices() const {
	return _use_indices;
}

n_vertices shape::get_number_vertices() const {
	return _number_vertices;
}

void shape::unbind() {
	glBindVertexArray(0);
}

unsigned int shape::create_vao() {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

unsigned int shape::buffer_vertices(const float* vertices, size_t vertices_size) {
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
	return vbo;
}

n_floats shape::get_attributes_size(const std::vector<attribute>& attributes) {
	unsigned int attributes_size = 0;
	for (const attribute& a : attributes)
	{
		attributes_size += a.size;
	}
	return attributes_size;
}

void shape::create_attribute_pointer(const std::vector<attribute>& attributes) {
	size_t attributes_stride = get_attributes_size(attributes) * sizeof(float);

	size_t offset = 0;
	for (unsigned int i = 0; i < attributes.size(); i++) {
		glVertexAttribPointer(
			i,
			attributes[i].size,
			attributes[i].type,
			GL_FALSE,
			attributes_stride,
			(void*)offset
		);
		offset += attributes[i].size * sizeof(float);
		glEnableVertexAttribArray(i);
	}
}

shape_specification shape::get_specification() const {
	return _specification;
}
