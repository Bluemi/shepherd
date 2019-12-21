#include "shape_heap.hpp"

#include "shape_initializer.hpp"

shape_heap::shape_heap() {}

shape_heap::~shape_heap() {
	for (auto& s : _shapes) {
		s.second->free_buffers();
		delete s.second;
	}
	_shapes.clear();
}

void shape_heap::load_shape(const shape_specification& spec) {
	auto it = _shapes.find(spec);
	if (it == _shapes.end()) {
		_shapes[spec] = new shape(initialize::create_shape(spec));
	}
}

shape shape_heap::get_shape(const shape_specification& spec) {
	auto it = _shapes.find(spec);
	shape* a_shape;

	if (it == _shapes.end()) {
		a_shape = new shape(initialize::create_shape(spec));
		_shapes[spec] = a_shape;
	} else {
		a_shape = it->second;
	}

	return shape(*a_shape);
}
