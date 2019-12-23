#ifndef __PLAYER_CLASS__
#define __PLAYER_CLASS__

#include <string>
#include <glm/glm.hpp>

class shader_program;

class player {
	public:
		player(unsigned int id, const std::string& name);

		unsigned int get_id() const;
		const std::string& get_name() const;
		const glm::vec3& get_position() const;
		const glm::vec3& get_speed() const;

		void set_name(const std::string& name);
		void set_position(const glm::vec3& position);
		void set_speed(const glm::vec3& speed);
	private:
		char _id;
		std::string _name;
		glm::vec3 _position;
		glm::vec3 _speed;
};

#endif
