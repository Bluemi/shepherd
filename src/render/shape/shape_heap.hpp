#ifndef __SHAPEHEAP_CLASS__
#define __SHAPEHEAP_CLASS__

#include <map>

#include "shape.hpp"
#include "shape_specification.hpp"

class shape_heap {
	public:
		shape_heap();
		~shape_heap();

		shape get_shape(const shape_specification& spec);
		void load_shape(const shape_specification& spec);
	private:
		std::map<shape_specification, shape*> _shapes;
};

#endif
