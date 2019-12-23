#ifndef __SERVER_CLASS__
#define __SERVER_CLASS__

#include <netsi/util/cycle.hpp>
#include <netsi/server/server_network_manager.hpp>

#include "../common/frame.hpp"

class server {
	public:
		server();

		void init();
		void run();
	private:
		void check_new_peers();
		void handle_clients();
		void handle_message(const std::vector<char>& message);
		void handle_login(const std::vector<char>& login_message);
		void send_game_update() const;

		netsi::server_network_manager _server_network_manager;
		std::vector<std::shared_ptr<netsi::peer>> _peers;
		frame _current_frame;
};

#endif
