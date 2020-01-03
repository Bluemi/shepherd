#ifndef __CONTROLLER_CLASS__
#define __CONTROLLER_CLASS__

#include <map>
#include <vector>

#include <glm/glm.hpp>

struct GLFWwindow;

class camera;
using Key = unsigned int;

class controller {
	public:
		controller();

		void mouse_callback(double x, double y);

		void process_user_input(GLFWwindow* window);

		void key_pressed(GLFWwindow* window, Key key);
		void key_released(GLFWwindow* window, Key key);

		bool is_key_pressed(const Key key) const;
		bool is_left_mouse_pressed() const;
		bool is_right_mouse_pressed() const;
		glm::vec2 poll_mouse_changes();

		static const Key CAMERA_FORWARD_KEY;
		static const Key CAMERA_BACKWARD_KEY;
		static const Key CAMERA_LEFT_KEY;
		static const Key CAMERA_RIGHT_KEY;
		static const Key CAMERA_TOP_KEY;
		static const Key CAMERA_BOTTOM_KEY;
		static const Key HOOK_KEY;
		static const Key CLOSE_KEY;
	private:

		std::map<Key, bool> _is_pressed;
		double _mouse_x_change, _mouse_y_change;
		bool _left_mouse_pressed;
		bool _right_mouse_pressed;
};

#endif
