#ifndef __PLAYER_CLASS__
#define __PLAYER_CLASS__

#include <string>
#include <glm/glm.hpp>

class player {
	public:
		player(unsigned int id, const std::string& name);
	private:
		unsigned int _id;
		std::string _name;
		glm::vec3 _position;
		glm::vec3 _speed;
};

#endif
