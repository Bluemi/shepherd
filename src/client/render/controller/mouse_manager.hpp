#ifndef __MOUSEMANAGER_CLASS__
#define __MOUSEMANAGER_CLASS__

#include <vector>

#include <GLFW/glfw3.h>

class controller;

namespace mouse_manager
{
	void mouse_callback(GLFWwindow* window, double x, double y);
	void init(GLFWwindow* window);
	void add_controller(controller* controller);
	void remove_controller(controller* controller);
	void clear();
}

#endif
