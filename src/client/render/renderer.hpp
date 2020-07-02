#ifndef __RENDERER_CLASS__
#define __RENDERER_CLASS__

#include <deque>

#include "controller/controller.hpp"
#include "shader_program.hpp"
#include "shape/shape_loader.hpp"

struct GLFWwindow;
class frame;
class block_chunk;

class renderer {
	public:
		renderer(
			GLFWwindow* window,
			shader_program player_shader_program,
			shader_program sheep_shader_program,
			shader_program block_shader_program,
			shader_program hook_shader_program,
			shader_program visor_shader_program,
			unsigned int window_width,
			unsigned int window_height
		);
		renderer(const renderer& visu);
		~renderer();

		static void init();
		static std::optional<renderer> create(unsigned int window_width, unsigned int window_height, const std::string& window_name);

		void run_shape_loader();
		void load_chunk(const block_chunk& bc);
		void load_next_chunk();
		void render_hook(const glm::vec3& player_position, const glm::vec3& hook_tip);
		void render(frame& f, char player_id);
		void close();
		bool should_close() const;

		double get_time() const;

		void framebuffer_size_callback(GLFWwindow*, int width, int height);
		GLFWwindow* get_window();

		const controller& get_controller() const;
		controller& get_controller();

	private:
		double get_delta_time();
		void clear_window();

		controller _controller;
		shader_program _player_shader_program;
		shader_program _sheep_shader_program;
		shader_program _block_shader_program;
		shader_program _hook_shader_program;
		shader_program _visor_shader_program;

		shape _player_shape;
		shape _sheep_shape;
		shape _hook_shape;
		shape _visor_shape;
		std::vector<render_chunk> _render_chunks;
		std::deque<chunk_request> _chunks_to_load;

		GLFWwindow* _window;

		double _last_frame_time;

		unsigned int _window_width;
		unsigned int _window_height;
		glm::vec3 _screen_position;
};

#endif
