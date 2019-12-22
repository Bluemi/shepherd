#ifndef __CLIENT_CLASS__
#define __CLIENT_CLASS__

#include <netsi/client/client_network_manager.hpp>

#include "../common/frame.hpp"
#include "render/renderer.hpp"

class client {
	public:
		client();
		void init(const std::string& player_name);
		void run();
	private:
		void send_login(const std::string& player_name);

		frame _current_frame;
		netsi::client_network_manager _network_manager;
		std::unique_ptr<renderer> _renderer;
		std::shared_ptr<netsi::peer> _peer;
};

#endif
