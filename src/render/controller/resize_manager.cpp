#include "resize_manager.hpp"

#include <vector>

#include <GLFW/glfw3.h>

#include "../renderer.hpp"

namespace resize_manager
{
	std::vector<renderer*> _renderers;

	void init(GLFWwindow* window) {
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	}

	void add_renderer(renderer* r) {
		_renderers.push_back(r);
	}

	void remove_renderer(renderer* r) {
		for (auto it = _renderers.cbegin(); it != _renderers.cend();) {
			if (*it == r) {
				it = _renderers.erase(it);
			} else {
				++it;
			}
		}
	}

	void clear_renderers() {
		_renderers.clear();
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		for (renderer* const r : _renderers) {
			r->framebuffer_size_callback(window, width, height);
		}
	}
}
