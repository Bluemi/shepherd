#ifndef __HOOK_CLASS__
#define __HOOK_CLASS__

#include <optional>
#include <glm/vec3.hpp>

#include "world/world_block.hpp"

class block_container;
class sheep;

constexpr float HOOK_RANGE = 15.f;
constexpr float HOOK_SPEED = 250.f;
constexpr float HOOK_ACCELERATION = 0.35f; 

class hook {
	public:
		hook();
		hook(const glm::vec3& position, const glm::vec3& direction);
		hook(const std::optional<glm::vec3>& target_point);

		bool is_hooked() const;
		void check_target(const block_container& blocks, std::vector<sheep>& sheeps);

		glm::vec3 position;
		glm::vec3 direction;
		float range;
		std::optional<glm::vec3> target_point;
		std::optional<unsigned int> target_sheep_index;
};

#endif
