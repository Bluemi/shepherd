#ifndef __RESIZEMANAGER_CLASS__
#define __RESIZEMANAGER_CLASS__

class GLFWwindow;
class renderer;

namespace resize_manager {
	void init(GLFWwindow* window);
	void add_renderer(renderer* rendr);
	void remove_renderer(renderer* rendr);
	void clear_renderers();
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}

#endif
