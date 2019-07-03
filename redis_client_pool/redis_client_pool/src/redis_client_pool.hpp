#ifndef REDIS_CLIENT_POOL_HPP_
#define REDIS_CLIENT_POOL_HPP_
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include "redis_client.hpp"
struct redis_client_pool_config {
	redis_client_pool_config() {
		poolsize_ = 32;
		port_ = 6379;
		ip_ = "127.0.0.1";
	}
	size_t poolsize_;
	string ip_;
	int port_;
};
using delete_type = function<void(redis_client *)>;
class redis_client_pool {
public:
	redis_client_pool() = default;
	virtual ~redis_client_pool() = default;
public:
	bool init(const redis_client_pool_config &config) {
		if (config.poolsize_ > POOLCAPACITY) {
			cerr << "pool size is over limit...!" << endl;
			return false;
		}
		for (size_t i = 0;i < config.poolsize_;i++) {
			unique_ptr<redis_client>ptr(new redis_client);
			if (!ptr->connect(config.ip_.c_str(), config.port_)) {
				return false;
			}
			clients_.push_back(move(ptr));
		}
		return true;
	}
	unique_ptr<redis_client, delete_type>get_client() {
		lock_guard<mutex>lock(pool_lock_);
		if (clients_.empty()) {
			return nullptr;
		}
		unique_ptr<redis_client, delete_type>ptr(clients_.back().release(), [this](redis_client *client) {
			pool_lock_.lock();
			clients_.push_back(unique_ptr<redis_client>(client));
			pool_lock_.unlock();
		});
		clients_.pop_back();
		return move(ptr);
	}

private:
	vector<unique_ptr<redis_client> >clients_;
	mutex pool_lock_;
private:
	static const size_t POOLCAPACITY = 128;


};

#endif /* REDIS_CLIENT_POOL_HPP_ */
