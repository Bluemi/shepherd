#ifndef __PLAYER_CLASS__
#define __PLAYER_CLASS__

#include <string>
#include <glm/glm.hpp>

#include "client/render/shape/shape.hpp"

class shader_program;

class player {
	public:
		player(unsigned int id, const std::string& name, const shape& shape);

		void render(const shader_program& shader_program);
	private:
		unsigned int _id;
		std::string _name;
		glm::vec3 _position;
		glm::vec3 _speed;
		shape _shape;
};

#endif
