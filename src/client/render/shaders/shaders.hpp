#ifndef __SHADERS_CLASS__
#define __SHADERS_CLASS__

#include <string>

namespace shaders {
	const std::string player_vertex_shader();
	const std::string player_fragment_shader();

	const std::string sheep_vertex_shader();
	const std::string sheep_fragment_shader();

	const std::string block_vertex_shader();
	const std::string block_fragment_shader();

	const std::string hook_vertex_shader();
	const std::string hook_fragment_shader();

	const std::string visor_vertex_shader();
	const std::string visor_fragment_shader();
}

#endif
