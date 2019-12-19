#include "shader_program.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.hpp"

const unsigned int LOG_BUFFER_SIZE = 512;

std::optional<shader_program> shader_program::from_files(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	std::optional<shader> opt_vertex_shader = shader::from_file(vertex_shader_path, GL_VERTEX_SHADER);
	std::optional<shader> opt_fragment_shader = shader::from_file(fragment_shader_path, GL_FRAGMENT_SHADER);

	if (!opt_vertex_shader) { return {}; }
	if (!opt_fragment_shader) { return {}; }

	shader vertex_shader = *opt_vertex_shader;
	shader fragment_shader = *opt_fragment_shader;

	return from_shaders(vertex_shader, fragment_shader);
}

std::optional<shader_program> shader_program::from_code(const std::string& vertex_shader_code, const std::string& fragment_shader_code)
{
	std::optional<shader> opt_vertex_shader = shader::from_code(vertex_shader_code, GL_VERTEX_SHADER);
	std::optional<shader> opt_fragment_shader = shader::from_code(fragment_shader_code, GL_FRAGMENT_SHADER);

	if (!opt_vertex_shader) { return {}; }
	if (!opt_fragment_shader) { return {}; }

	shader vertex_shader = *opt_vertex_shader;
	shader fragment_shader = *opt_fragment_shader;

	return from_shaders(vertex_shader, fragment_shader);
}

std::optional<shader_program> shader_program::from_shaders(const shader& vertex_shader, const shader& fragment_shader)
{
	const unsigned int id = glCreateProgram();
	glAttachShader(id, vertex_shader.get_id());
	glAttachShader(id, fragment_shader.get_id());
	glLinkProgram(id);

	int success;
	char infoLog[LOG_BUFFER_SIZE];

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(id, LOG_BUFFER_SIZE, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return {};
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	vertex_shader.del();
	fragment_shader.del();

	return shader_program(id);
}

shader_program::shader_program(unsigned int id)
	: _id(id)
{}

void shader_program::use() const
{
	glUseProgram(_id);
}

unsigned int shader_program::get_id() const
{
	return _id;
}

int shader_program::get_uniform_location(const std::string& name) const
{
	int loc = glGetUniformLocation(_id, name.c_str());
	if (loc < 0)
	{
		std::cout << "ERROR: invalid uniform name: \"" << name << "\"" << std::endl;
	}
	return loc;
}

void shader_program::set_bool(const std::string& name, bool value) const
{
	int loc = get_uniform_location(name);
	glUniform1i(loc, (int)value);
}

void shader_program::set_float(const std::string& name, float value) const
{
	int loc = get_uniform_location(name);
	glUniform1f(loc, value);
}

void shader_program::set_int(const std::string& name, int value) const
{
	int loc = get_uniform_location(name);
	glUniform1i(loc, value);
}

void shader_program::set_2f(const std::string& name, const glm::vec2& value) const
{
	int loc = get_uniform_location(name);
	glUniform2f(loc, value.x, value.y);
}

void shader_program::set_3f(const std::string& name, const glm::vec3& value) const
{
	int loc = get_uniform_location(name);
	glUniform3f(loc, value.x, value.y, value.z);
}

void shader_program::set_4f(const std::string& name, const glm::vec4& value) const
{
	int loc = get_uniform_location(name);
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void shader_program::set_4fv(const std::string& name, const glm::mat4& mat) const
{
	int loc = get_uniform_location(name);
	glUniformMatrix4fv(loc, 1,GL_FALSE, glm::value_ptr(mat));
}
