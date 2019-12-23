#ifndef __RENDERER_CLASS__
#define __RENDERER_CLASS__

#include "controller/controller.hpp"
#include "shader_program.hpp"
#include "shape/shape_heap.hpp"

struct GLFWwindow;
class frame;

class renderer {
	public:
		renderer(GLFWwindow* window, shader_program shader_program, unsigned int window_width, unsigned int window_height);
		renderer(const renderer& visu);
		~renderer();

		static void init();
		static std::optional<renderer> create(unsigned int window_width, unsigned int window_height, const std::string& window_name);

		void tick();
		void render(frame& f, char player_id);
		void close();
		bool should_close() const;

		double get_time() const;

		void framebuffer_size_callback(GLFWwindow*, int width, int height);

		const controller& get_controller() const;
		controller& get_controller();
	private:
		double get_delta_time();
		void clear_window();

		controller _controller;
		shader_program _shader_program;

		shape _player_shape;

		GLFWwindow* _window;

		double _last_frame_time;

		unsigned int _window_width;
		unsigned int _window_height;

};

#endif
