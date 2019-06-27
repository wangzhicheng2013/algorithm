#include "udp_server.hpp"
int main() {
	socket_config config;
	udp_server server(config);
	cout << server.init(thread::hardware_concurrency()) << endl;

	return 0;
}
