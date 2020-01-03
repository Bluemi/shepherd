#ifndef __PLAYER_CLASS__
#define __PLAYER_CLASS__

#include <vector>
#include <optional>
#include <string>

#include "world/block_container.hpp"
#include "physics/forms.hpp"
#include "hook.hpp"

class player {
	public:
		player(unsigned int id, const std::string& name);
		player(unsigned int id, const std::string& name, const glm::vec3& position);
		player(unsigned int id, const std::string& name, const glm::vec3& position, const std::optional<glm::vec3>& h);

		unsigned int get_id() const;
		const std::string& get_name() const;
		const glm::vec3& get_position() const;
		const glm::vec2& get_view_angles() const;
		const glm::vec3& get_speed() const;
		std::uint16_t get_actions() const;
		bool poll_left_mouse_pressed();
		bool poll_right_mouse_pressed();
		glm::vec3 get_color() const;
		const std::optional<hook>& get_hook() const;
		bool is_hooked() const;

		void set_name(const std::string& name);
		void set_position(const glm::vec3& position);
		void set_view_angles(const glm::vec2& view_angles);
		void set_speed(const glm::vec3& speed);
		void set_actions(const std::uint16_t actions);
		void set_hook(const std::optional<hook>& h);
		void update_direction(const glm::vec2& direction_update);

		static glm::vec3 get_up();
		glm::vec3 get_right() const;
		glm::vec3 get_direction() const;
		glm::vec3 get_top() const;
		glm::mat4 get_look_at() const;
		glm::vec3 get_camera_position() const;

		void respawn(const glm::vec3& position);
		bool tick(const block_container& blocks);
		void apply_player_movements(const block_container& blocks);
		void apply_drag(glm::vec3& speed);
		void physics(const block_container& blocks);
		void check_collider(const block_container& blocks, const cuboid& collider, int direction, unsigned int coordinate);
		void handle_hook(const block_container& blocks);
		void handle_active_hook(const block_container& blocks);

		cuboid get_bottom_collider() const;
		cuboid get_top_collider() const;
		cuboid get_left_collider() const;
		cuboid get_right_collider() const;
		cuboid get_front_collider() const;
		cuboid get_back_collider() const;
	private:
		char _id;
		std::string _name;
		glm::vec3 _position;
		glm::vec3 _size;
		glm::vec2 _view_angles;
		glm::vec3 _speed;
		std::uint16_t _actions;
		glm::vec3 _color;
		bool _on_left_mouse_pressed;
		bool _on_right_mouse_pressed;

		std::optional<hook> _hook;
};

#endif
