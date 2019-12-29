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

renderer::renderer(GLFWwindow* window, shader_program player_shader_program, shader_program block_shader_program, unsigned int window_width, unsigned int window_height)
	: _player_shader_program(player_shader_program),
	  _block_shader_program(block_shader_program),
	  _window(window),
	  _last_frame_time(0.0),
	  _window_width(window_width),
	  _window_height(window_height)
{
	resize_manager::init(_window);
	resize_manager::add_renderer(this);

	mouse_manager::init(_window);
	mouse_manager::add_controller(&_controller);

	_player_shape = initialize::create_shape(sphere_specification(3));
	_world_block_shape = initialize::create_shape(cube_specification());
}

renderer::renderer(const renderer& v)
	: _controller(v._controller),
	  _player_shader_program(v._player_shader_program),
	  _block_shader_program(v._block_shader_program),
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

	_player_shape.free_buffers();
	_block_shape.free_buffers();
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

	std::optional<shader_program> opt_player_shader_program = shader_program::from_code(shaders::vertex_shader(), shaders::player_fragment_shader());

	if (!opt_player_shader_program) {
		std::cerr << "failed to create player shader program" << std::endl;
		return {};
	}

	std::optional<shader_program> opt_block_shader_program = shader_program::from_code(shaders::vertex_shader(), shaders::block_fragment_shader());

	if (!opt_block_shader_program) {
		std::cerr << "failed to create block shader program" << std::endl;
		return {};
	}

	return renderer(window, *opt_player_shader_program, *opt_block_shader_program, window_width, window_height);
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
		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			_window_width/static_cast<float>(_window_height),
			0.1f, 600.f
		);

		// render players
		_player_shader_program.set_4fv("view", local_player->get_look_at());
		_player_shader_program.set_4fv("projection", projection);
		_player_shader_program.use();

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
			_player_shader_program.set_4fv("model", model);

			_player_shader_program.set_3f("color", p.get_color());

			glDrawArrays(GL_TRIANGLES, 0, _player_shape.get_number_vertices());
		}

		// render blocks
		_block_shader_program.set_4fv("view", local_player->get_look_at());
		_block_shader_program.set_4fv("projection", projection);
		_block_shader_program.use();

		_world_block_shape.bind();

		for (const std::shared_ptr<block_chunk>& bc : f.blocks.get_chunks()) {
			for (unsigned int x = 0; x < BLOCK_CHUNK_SIZE; x++) {
				for (unsigned int y = 0; y < BLOCK_CHUNK_SIZE; y++) {
					for (unsigned int z = 0; z < BLOCK_CHUNK_SIZE; z++) {
						glm::ivec3 pos = glm::ivec3(x, y, z) + bc->get_origin();
						block_type bt = bc->get_block_type(pos);
						if (bt != block_type::VOID) {
							glm::mat4 model = glm::mat4(1.f);
							model = glm::translate(model, glm::vec3(pos));
							_block_shader_program.set_4fv("model", model);

							if (bt == block_type::WINNING) {
								_block_shader_program.set_3f("color", block_container::get_winning_color(pos));
							} else {
								_block_shader_program.set_3f("color", block_container::get_color(pos));
							}

							glDrawArrays(GL_TRIANGLES, 0, _world_block_shape.get_number_vertices());
						}
					}
				}
			}
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
