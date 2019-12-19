#ifndef __CAMERA_CLASS__
#define __CAMERA_CLASS__

#include <glm/glm.hpp>

class camera
{
	public:
		camera();
		camera(const glm::vec3 &position, const float pitch, const float yaw);

		void tick(const double speed);
		void stop();

		static glm::vec3 get_up();
		glm::vec3 get_right() const;
		glm::vec3 get_direction() const;
		glm::vec3 get_top() const;
		glm::mat4 get_look_at() const;

		glm::vec3 get_position() const;

		void change_direction(glm::vec2 value);
		void set_acceleration(const glm::vec3& acceleration);
	private:
		static const float CAMERA_SPEED;
		static const float CAMERA_ROTATE_SPEED;
		static const float CAMERA_DRAG;

		glm::vec3 _position;
		glm::vec3 _speed;

		float _pitch;
		float _yaw;

		glm::vec3 _acceleration;
};

#endif
