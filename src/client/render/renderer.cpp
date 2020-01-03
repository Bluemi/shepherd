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
	_hook_shape = initialize::create_shape(cube_specification());
}

renderer::renderer(const renderer& v)
	: _controller(v._controller),
	  _player_shader_program(v._player_shader_program),
	  _block_shader_program(v._block_shader_program),
	  _player_shape(v._player_shape),
	  _hook_shape(v._hook_shape),
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

	std::optional<shader_program> opt_player_shader_program = shader_program::from_code(shaders::player_vertex_shader(), shaders::player_fragment_shader());

	if (!opt_player_shader_program) {
		std::cerr << "failed to create player shader program" << std::endl;
		return {};
	}

	std::optional<shader_program> opt_block_shader_program = shader_program::from_code(shaders::block_vertex_shader(), shaders::block_fragment_shader());

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

void renderer::load_chunk(const block_chunk& bc) {
	chunk_request cr(bc.get_block_types(), bc.get_origin());

	_chunks_to_load.push_back(cr);
	
}

void renderer::load_next_chunk() {
	if (!_chunks_to_load.empty()) {
		chunk_request cr = _chunks_to_load.front();
		_chunks_to_load.pop_front();

		// remove old chunk
		for (auto it = _render_chunks.begin(); it != _render_chunks.end(); ++it) {
			if (it->origin == cr.origin) {
				it->chunk_shape.free_buffers();
				_render_chunks.erase(it);
				break;
			}
		}

		render_chunk rc = do_load_chunk(cr);
		_render_chunks.push_back(rc);
	}
}

void renderer::tick() {
	const double speed = get_delta_time() * DEFAULT_SPEED;

	_controller.process_user_input(_window);
}

void renderer::render_hook(const glm::vec3& player_position, const glm::vec3& hook_tip) {
	const glm::vec3 mid = (player_position + hook_tip) / 2.f;
	const glm::vec3 from_to = hook_tip - player_position;
	const float range = glm::distance(player_position, hook_tip) + 0.0001f;

	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, mid);
	// model = glm::rotate(model, glm::radians(p.get_view_angles().x), p.get_right());
	
	float up_angle = glm::sign(from_to.z) * glm::sign(from_to.x) * glm::asin(glm::abs(from_to.z) / range);
	float side_angle = glm::sign(from_to.y) * glm::sign(from_to.x) * glm::asin(glm::abs(from_to.y) / range);

	model = glm::rotate(model, -up_angle, player::get_up());
	model = glm::rotate(model, side_angle, glm::vec3(0.f, 0.f, 1.f));

	model = glm::scale(model, glm::vec3(range, 0.1f, 0.1f));
	_player_shader_program.set_4fv("model", model);

	_player_shader_program.set_3f("color", glm::vec3());

	glDrawArrays(GL_TRIANGLES, 0, _hook_shape.get_number_vertices());
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

		// render hooks
		_hook_shape.bind();

		for (player& p : f.players) {
			if (!p.get_hook()) continue;
			if (!p.get_hook()->target_block) continue;
			render_hook(p.get_position(), *p.get_hook()->target_block);
		}

		// render blocks
		_block_shader_program.set_4fv("view", local_player->get_look_at());
		_block_shader_program.set_4fv("projection", projection);
		_block_shader_program.set_3f("color", glm::vec3(0.2, 0.2, 0.2));
		_block_shader_program.use();

		for (const render_chunk& rc : _render_chunks) {
			rc.chunk_shape.bind();
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, glm::vec3(rc.origin));
			_block_shader_program.set_4fv("model", model);

			glDrawArrays(GL_TRIANGLES, 0, rc.chunk_shape.get_number_vertices());
		}
	}

	glfwSwapBuffers(_window);
	glfwPollEvents();

	load_next_chunk();
}

void renderer::close() {
	mouse_manager::clear();
	resize_manager::clear_renderers();

	_player_shape.free_buffers();
	_hook_shape.free_buffers();

	for (render_chunk& rc : _render_chunks) {
		rc.chunk_shape.free_buffers();
	}

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
