#ifndef __CLIENT_CLASS__
#define __CLIENT_CLASS__

#include <netsi/client/client_network_manager.hpp>

#include "../frame.hpp"
#include "render/renderer.hpp"

class client {
	public:
		client();
		void init();
		void run();
	private:
		frame _current_frame;
		netsi::client_network_manager _network_manager;
		std::unique_ptr<renderer> _renderer;
};

#endif
