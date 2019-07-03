#include "redis_client_pool.hpp"
int main() {
	redis_client_pool mypool;
	redis_client_pool_config config;
	if (false == mypool.init(config)) {
		return -1;
	}
	const char *key = "key0";
	string value = "value0";
	auto client_ptr = mypool.get_client();
	if (nullptr == client_ptr) {
		return -1;
	}
	if (client_ptr->set_value(key, value)) {
		cout << "set value to redis server ok." << endl;
	}
	string new_value;
	if (client_ptr->get_value(key, new_value)) {
		cout << "get value = " << new_value << " from redis server ok." << endl;
	}

	return 0;
}
