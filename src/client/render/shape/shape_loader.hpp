#ifndef __SHAPE_LOADER_CLASS__
#define __SHAPE_LOADER_CLASS__

#include <optional>
#include <boost/thread.hpp>

#include <glm/vec3.hpp>
#include <netsi/util/blocking_queue.hpp>

#include "shape.hpp"
#include "../../../common/world/world_block.hpp"

class block_chunk;

struct render_chunk {
	render_chunk(const shape& s, const glm::ivec3& o) : chunk_shape(s), origin(o) {}

	shape chunk_shape;
	glm::ivec3 origin;
};

struct chunk_request {
	chunk_request() {}
	chunk_request(const std::vector<block_type>& b, const glm::ivec3& o) : blocks(b), origin(o) {}

	std::vector<block_type> blocks;
	glm::ivec3 origin;
};

class shape_loader {
	public:
		shape_loader();

		void run();
		void stop();
		void operator()();

		void load_chunk(const block_chunk& chunk);

		netsi::blocking_queue<render_chunk>& get_update_queue();
	private:
		void load_next_chunk();
		static void add_block(const glm::uvec3& position, const glm::ivec3& origin, std::vector<float>* vertices);

		netsi::blocking_queue<render_chunk> _shape_queue;
		netsi::blocking_queue<chunk_request> _request_queue;
		std::optional<boost::thread> _run_thread;
		std::atomic<bool> _running;
};

#endif
