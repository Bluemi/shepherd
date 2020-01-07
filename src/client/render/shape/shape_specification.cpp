#include "shape_specification.hpp"

#include "shape_initializer.hpp"

sphere_specification::sphere_specification(unsigned int fineness)
	: _fineness(fineness)
{}

bool sphere_specification::operator==(const sphere_specification& spec) const {
	return _fineness == spec._fineness;
}

unsigned int sphere_specification::get_fineness() const {
	return _fineness;
}

class {
	public:
		shape operator()(const cube_specification&) {
			return initialize::cube();
		}

		shape operator()(const sphere_specification& spec) {
			return initialize::sphere(spec.get_fineness());
		}

		shape operator()(const sheep_specification&) {
			return initialize::sheep();
		}
} _shape_extractor;

namespace initialize {
	shape create_shape(const shape_specification& spec) {
		return std::visit(_shape_extractor, spec);
	}
}
