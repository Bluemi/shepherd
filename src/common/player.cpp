#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

player::player(unsigned int id, const std::string& name): _id(id), _name(name) {}

/*
void player::render(const shader_program& shader_program) {
	shader_program.use();
	_shape.bind();

	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, _position);
	shader_program.set_4fv("model", model);

	shader_program.set_3f("color", glm::vec3(0.2, 0.2, 0.5));

	glDrawArrays(GL_TRIANGLES, 0, _shape.get_number_vertices());
}
*/

unsigned int player::get_id() const {
	return _id;
}

const std::string& player::get_name() const {
	return _name;
}

const glm::vec3& player::get_position() const {
	return _position;
}

const glm::vec3& player::get_speed() const {
	return _speed;
}
