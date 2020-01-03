#include "hook.hpp"

#include <glm/gtx/norm.hpp>

#include "world/block_container.hpp"
#include "physics/forms.hpp"

hook::hook() {}

hook::hook(const glm::vec3& p, const glm::vec3& d)
	: position(p), direction(d), range(0.f)
{}

hook::hook(const std::optional<glm::vec3>& tp)
	: target_point(tp)
{}

bool hook::is_hooked() const {
	return target_point.has_value();
}

void hook::check_target_point(const block_container& blocks) {
	if (!is_hooked()) {
		std::optional<glm::vec3> cp = blocks.get_collision_point(ray(position, direction), range);
		if (cp) {
			if (glm::distance2(*cp, position) <= HOOK_RANGE*HOOK_RANGE) {
				target_point = cp;
			}
		}
	}
}
