#ifndef __HOOK_CLASS__
#define __HOOK_CLASS__

#include <optional>
#include <glm/vec3.hpp>

#include "world/world_block.hpp"

class block_container;

constexpr float HOOK_RANGE = 15.f;
constexpr float HOOK_SPEED = 250.f;
constexpr float HOOK_ACCELERATION = 0.35f; 

class hook {
	public:
		hook(const glm::vec3& position, const glm::vec3& direction);

		bool is_hooked() const;
		void check_target_block(const block_container& blocks);

		glm::vec3 position;
		glm::vec3 direction;
		float range;
		std::optional<world_block> target_block;
};

#endif
