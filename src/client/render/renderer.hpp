#ifndef __RENDERER_CLASS__
#define __RENDERER_CLASS__

#include "controller/controller.hpp"
#include "camera/camera.hpp"
#include "shader_program.hpp"

struct GLFWwindow;

class renderer {
	public:
		renderer(GLFWwindow* window, shader_program shader_program, unsigned int window_width, unsigned int window_height);
		renderer(const renderer& visu);
		~renderer();

		static void init();
		static std::optional<renderer> create(unsigned int window_width, unsigned int window_height, const std::string& window_name);

		void tick();
		void render();
		void close();
		bool should_close() const;

		double get_time() const;

		void framebuffer_size_callback(GLFWwindow*, int width, int height);
	private:
		double get_delta_time();
		void clear_window();

		controller _controller;
		camera _camera;
		shader_program _shader_program;

		GLFWwindow* _window;

		double _last_frame_time;

		unsigned int _window_width;
		unsigned int _window_height;
};

#endif
