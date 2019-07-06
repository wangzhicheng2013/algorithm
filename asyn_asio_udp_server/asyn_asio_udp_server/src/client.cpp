#include "asyn_asio_udp_client.hpp"
int main() {
	asyn_asio_udp_client client("I love you.");
	client.start();

	return 0;
}
