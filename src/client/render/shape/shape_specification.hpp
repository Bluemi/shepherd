#ifndef __SHAPESPECIFICATION_CLASS__
#define __SHAPESPECIFICATION_CLASS__

#include <variant>

class shape;

/**
 * Helper class to provide relation operators.
 * No polymorphism intended.
 */
template<typename Specification>
class specification_operators {
	public:
		bool operator==(const Specification&) const { return true; }
		bool operator<(const Specification&) const { return false; }
		bool operator<=(const Specification&) const { return true; }
		bool operator>(const Specification&) const { return false; }
		bool operator>=(const Specification&) const { return true; }
};

class cube_specification : public specification_operators<cube_specification> {};

class sphere_specification : public specification_operators<sphere_specification> {
	public:
		sphere_specification(unsigned int fineness);
		bool operator==(const sphere_specification& spec) const;

		unsigned int get_fineness() const;
	private:
		unsigned int _fineness;
};

using shape_specification = std::variant<cube_specification, sphere_specification>;

namespace initialize {
	shape create_shape(const shape_specification& spec);
}

#endif
