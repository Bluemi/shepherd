#include "renderer.hpp"

#include <iostream>
#include <cmath>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shape/shape_initializer.hpp"
#include "controller/mouse_manager.hpp"
#include "controller/resize_manager.hpp"
#include "shaders/shaders.hpp"
#include "../../common/frame.hpp"
#include "../../common/physics/util.hpp"

const static std::string VERTEX_SHADER_PATH = "src/client/render/shaders/vertex_shader.vs";
const static std::string FRAGMENT_SHADER_PATH = "src/client/render/shaders/fragment_shader.fs";
const static double DEFAULT_SPEED = 59.54188473881952259316;

renderer::renderer(GLFWwindow* window, shader_program shader_program, unsigned int window_width, unsigned int window_height)
	: _shader_program(shader_program),
	  _window(window),
	  _last_frame_time(0.0),
	  _window_width(window_width),
	  _window_height(window_height)
{
	resize_manager::init(_window);
	resize_manager::add_renderer(this);

	mouse_manager::init(_window);
	mouse_manager::add_controller(&_controller);

	_player_shape = initialize::create_shape(sphere_specification(2));
	_world_block_shape = initialize::create_shape(cube_specification());
}

renderer::renderer(const renderer& v)
	: _controller(v._controller),
	  _shader_program(v._shader_program),
	  _player_shape(v._player_shape),
	  _world_block_shape(v._world_block_shape),
	  _window(v._window),
	  _last_frame_time(v._last_frame_time),
	  _window_width(v._window_width),
	  _window_height(v._window_height)
{
	resize_manager::add_renderer(this);
	mouse_manager::add_controller(&_controller);
}

renderer::~renderer() {
	resize_manager::remove_renderer(this);
	mouse_manager::remove_controller(&_controller);
}

void renderer::framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	_window_width = width;
	_window_height = height;
	glViewport(0, 0, width, height);
}

void renderer::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

std::optional<renderer> renderer::create(unsigned int window_width, unsigned int window_height, const std::string& window_name)
{
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_name.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "failed to create window" << std::endl;
		glfwTerminate();
		return {};
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return {};
	}

	glViewport(0, 0, window_width, window_height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	std::optional<shader_program> opt_shader_program = shader_program::from_code(shaders::vertex_shader(), shaders::fragment_shader());

	if (!opt_shader_program)
	{
		return {};
	}

	return renderer(window, *opt_shader_program, window_width, window_height);
}

double renderer::get_delta_time()
{
	double delta_time = 0.0;
	if (_last_frame_time == 0.0)
	{
		glfwSetTime(0.0);
	} else {
		delta_time = glfwGetTime() - _last_frame_time;
	}
	_last_frame_time = glfwGetTime();
	return delta_time;
}

void renderer::tick() {
	const double speed = get_delta_time() * DEFAULT_SPEED;

	_controller.process_user_input(_window);
}

void renderer::render(frame& f, char local_player_id) {
	clear_window();
	player* local_player = f.get_player(local_player_id);

	if (local_player != nullptr) {
		_shader_program.set_4fv("view", local_player->get_look_at());
		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			_window_width/static_cast<float>(_window_height),
			0.1f, 600.f
		);
		_shader_program.set_4fv("projection", projection);

		_shader_program.use();

		// render players
		_player_shape.bind();

		for (player& p : f.players) {
			// dont render local player
			if (p.get_id() == local_player_id) {
				continue;
			}
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, p.get_position());
			model = glm::rotate(model, glm::radians(p.get_view_angles().x), p.get_right());
			model = glm::rotate(model, glm::radians(-p.get_view_angles().y), player::get_up());
			_shader_program.set_4fv("model", model);

			_shader_program.set_3f("color", glm::vec3(0.2, 0.2, 0.5));

			glDrawArrays(GL_TRIANGLES, 0, _player_shape.get_number_vertices());
		}

		// render blocks
		_world_block_shape.bind();

		for (const world_block& b : f.blocks.get_blocks()) {
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, b.get_position());
			_shader_program.set_4fv("model", model);

			_shader_program.set_3f("color", glm::vec3(0.5, 0.2, 0.2));

			glDrawArrays(GL_TRIANGLES, 0, _world_block_shape.get_number_vertices());
		}
	}

	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void renderer::close() {
	mouse_manager::clear();
	resize_manager::clear_renderers();

	glfwTerminate();
}

bool renderer::should_close() const {
	return glfwWindowShouldClose(_window);
}

double renderer::get_time() const {
	return glfwGetTime();
}

void renderer::clear_window() {
		glClearColor(0.05f, 0.07f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

const controller& renderer::get_controller() const {
	return _controller;
}

controller& renderer::get_controller() {
	return _controller;
}
