#include "shaders.hpp"

/*
 * This define is used to embed the shader code into the executable
 * without having to write the code inside a cpp/hpp file.
 *
 * It is used in the shader files.
 */
#define VISUALIZER_SHADER_STRINGIFY(x) #x

namespace shaders {
	// The version prefix is excluded from the shader code,
	// because otherwise it is interpreted as precompiler directive.
	const std::string VERSION_PREFIX = "#version 330\n";

	const std::string player_vertex_shader() {
		std::string s =
		#include "player_vertex_shader.vs"
		;
		return VERSION_PREFIX + s;
	}

	const std::string player_fragment_shader() {
		std::string s =
		#include "player_fragment_shader.fs"
		;
		return VERSION_PREFIX + s;
	}

	const std::string block_vertex_shader() {
		std::string s =
		#include "block_vertex_shader.vs"
		;
		return VERSION_PREFIX + s;
	}

	const std::string block_fragment_shader() {
		std::string s =
		#include "block_fragment_shader.fs"
		;
		return VERSION_PREFIX + s;
	}

	const std::string hook_vertex_shader() {
		std::string s =
		#include "hook_vertex_shader.vs"
		;
		return VERSION_PREFIX + s;
	}

	const std::string hook_fragment_shader() {
		std::string s =
		#include "hook_fragment_shader.fs"
		;
		return VERSION_PREFIX + s;
	}
}
