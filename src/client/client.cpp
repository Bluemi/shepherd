#include "client.hpp"

client::client() {}

void client::init() {
	renderer::init();
}

void client::run() {
	std::optional<renderer> opt_renderer = renderer::create(800, 600, "cube racing");
	if (!opt_renderer) {
		std::cerr << "Failed to initalizer renderer" << std::endl;
		return;
	}

	_renderer = std::make_unique<renderer>(*opt_renderer);

	while (!_renderer->should_close()) {
		_renderer->render();
	}

	_renderer->close();
}

int main() {
	client c;
	c.init();
	c.run();
	return 0;
}
