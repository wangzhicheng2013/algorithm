#ifndef UDP_SERVER_HPP_
#define UDP_SERVER_HPP_
#include <vector>
#include <thread>
#include <memory>
#include "udp_thread_for_transaction.hpp"

class udp_server {
public:
	udp_server(const socket_config &config) : sock_fd_(-1), config_(config) {
	}
	virtual ~udp_server() {
		for (auto &it : threads_) {
			if (it.joinable()) {
				it.join();
			}
		}
		sockect_utility::get_instance().close_socket(sock_fd_);
	}
public:
	bool init(int network_size) {
		if (network_size <= 0 || network_size > network_capacity) {
			return false;
		}
		sock_fd_ = sockect_utility::get_instance().bind_socket(config_);
		if (sock_fd_ < 0) {
			return false;
		}
		threads_.resize(network_size);
		for (int i = 0;i < network_size;i++) {
			udp_threads_.emplace_back(unique_ptr<udp_thread>(new udp_thead_for_transaction));
			if (false == udp_threads_[i]->init(sock_fd_)) {
				cerr << "udp thread init failed...!" << endl;
				continue;
			}
			try {
				threads_[i] = thread(std::bind(&udp_server::start_udp_thread, this, i));
			}
			catch (...) {
				return false;
			}
		}
		return true;
	}
private:
	void start_udp_thread(int index) {
		udp_threads_[index]->udp_dispatch_event();
	}
private:
	socket_config config_;
	vector<unique_ptr<udp_thread> >udp_threads_;
	vector<thread>threads_;
private:
	int sock_fd_;
private:
	const int network_capacity = 128;
};

#endif /* UDP_SERVER_HPP_ */
