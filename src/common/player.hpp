#ifndef __PLAYER_CLASS__
#define __PLAYER_CLASS__

#include <vector>
#include <optional>
#include <string>

#include "world/block_container.hpp"
#include "physics/forms.hpp"
#include "physics/body.hpp"
#include "hook.hpp"

class player {
	public:
		player(unsigned int id, const std::string& name);
		player(unsigned int id, const std::string& name, const glm::vec3& position);
		player(unsigned int id, const std::string& name, const glm::vec3& position, const std::optional<glm::vec3>& h);

		char get_id() const;
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
		bool is_block_hooked() const;

		void set_name(const std::string& name);
		void set_position(const glm::vec3& position);
		void set_view_angles(const glm::vec2& view_angles);
		void set_speed(const glm::vec3& speed);
		void set_actions(const std::uint16_t actions);
		void set_hook(const std::optional<hook>& h);
		void reset_hook(std::vector<sheep>& sheeps);
		void update_direction(const glm::vec2& direction_update);

		glm::vec3 get_right() const;
		glm::vec3 get_direction() const;
		glm::vec3 get_top() const;
		glm::mat4 get_look_at() const;
		glm::vec3 get_camera_position() const;

		void respawn(const glm::vec3& position, std::vector<sheep>& sheeps);
		bool tick(const block_container& blocks, std::vector<sheep>& sheeps);
		void apply_player_movements(const block_container& blocks);
		void physics(const block_container& blocks);
		void handle_hook(const block_container& blocks, std::vector<sheep>& sheeps);
		void handle_active_hook(const block_container& blocks, std::vector<sheep>& sheeps);
	private:
		char _id;

		std::string _name;
		body _body;

		std::uint16_t _actions;
		glm::vec3 _color;
		bool _on_left_mouse_pressed;
		bool _on_right_mouse_pressed;

		std::optional<hook> _hook;
};

#endif
