#include "hook.hpp"

#include <glm/gtx/norm.hpp>

#include "world/block_container.hpp"
#include "physics/forms.hpp"

hook::hook(const glm::vec3& p, const glm::vec3& d)
	: position(p), direction(d), range(0.f)
{}

bool hook::is_hooked() const {
	return target_block.has_value();
}

void hook::check_target_block(const block_container& blocks) {
	if (!is_hooked()) {
		std::optional<world_block> tb = blocks.get_colliding_block(ray(position, direction), range);
		if (tb) {
			if (glm::distance2(glm::vec3(tb->get_position()), position) <= HOOK_RANGE*HOOK_RANGE) {
				target_block = tb;
			}
		}
	}
}
