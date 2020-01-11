#include "hook.hpp"

#include <glm/gtx/norm.hpp>

#include "world/block_container.hpp"
#include "sheep.hpp"
#include "physics/forms.hpp"

hook::hook() {}

hook::hook(const glm::vec3& p, const glm::vec3& d)
	: position(p), direction(d), range(0.f)
{}

hook::hook(const std::optional<glm::vec3>& tp)
	: target_point(tp)
{}

bool hook::is_hooked() const {
	return target_point.has_value() || target_sheep_index.has_value();
}

void hook::check_target(const block_container& blocks, const std::vector<sheep>& sheeps) {
	if (!is_hooked()) {
		std::optional<glm::vec3> cp = blocks.get_collision_point(ray(position, direction), range);
		if (cp) {
			if (glm::distance2(*cp, position) <= HOOK_RANGE*HOOK_RANGE) {
				target_point = cp;
			}
		}

		glm::vec3 sheep_position;
		int sheep_index(-1);
		for (unsigned int i = 0; i < sheeps.size(); i++) {
			if (sheeps[i].is_colliding(ray(position, direction), range)) {
				if ((sheep_index != -1) && glm::distance2(position, sheeps[i].get_position()) > glm::distance2(position, sheep_position)) continue;
				sheep_position = sheeps[i].get_position();
				sheep_index = i;
			}
		}

		if (sheep_index != -1) {
			if (glm::distance2(*cp, position) > glm::distance2(sheep_position, position)) {
				target_point.reset();
				target_sheep_index = sheep_index;
			}
		}
	}
}
