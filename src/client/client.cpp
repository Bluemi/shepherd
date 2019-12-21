#include "client.hpp"

client::client() {}

void client::init() {
	renderer::init();

	std::optional<renderer> opt_renderer = renderer::create(800, 600, "cube racing");
	if (!opt_renderer) {
		std::cerr << "Failed to initalizer renderer" << std::endl;
		return;
	}

	_renderer = std::make_unique<renderer>(*opt_renderer);

	_current_frame.add_player();
}

void client::run() {
	while (!_renderer->should_close()) {
		_renderer->tick();
		_renderer->render(_current_frame);
	}

	_renderer->close();
}

int main() {
	client c;
	c.init();
	c.run();
	return 0;
}
