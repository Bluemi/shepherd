#include "player.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "client/render/shader_program.hpp"

player::player(unsigned int id, const std::string& name, const shape& shape): _id(id), _name(name), _shape(shape) {}

void player::render(const shader_program& shader_program) {
	shader_program.use();
	_shape.bind();

	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, _position);
	shader_program.set_4fv("model", model);

	shader_program.set_3f("color", glm::vec3(0.5, 0.5, 0.5));

	glDrawArrays(GL_TRIANGLES, 0, _shape.get_number_vertices());
}
