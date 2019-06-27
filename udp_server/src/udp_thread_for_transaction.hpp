#ifndef UDP_THREAD_FOR_TRANSACTION_HPP_
#define UDP_THREAD_FOR_TRANSACTION_HPP_
#include "udp_thread.hpp"
class udp_thead_for_transaction : public udp_thread {
public:
	udp_thead_for_transaction() = default;
	virtual ~udp_thead_for_transaction() = default;
public:
	virtual bool deal_udp_message(int fd) {
		char buf[buffer_size] = "";
		socklen_t size = sizeof(struct sockaddr);
		struct sockaddr_in client_addr = {0};
		int len = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &size);
		if (len < 0) {
			cerr << "server recv message error...!" << endl;
			return false;
		}
		if (0 == len) {
			cerr << "connection closed...!" << endl;
		}
		cout << "connection port = " << client_addr.sin_port << endl;
		cout << "connection ip = " << inet_ntoa(client_addr.sin_addr) << endl;
		cout << "server recv message len = " << len << endl;
		cout << "sever send back message now...!" << endl;
		sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, size);
		return true;
	}
};


#endif /* UDP_THREAD_FOR_TRANSACTION_HPP_ */
