g++ -std=c++11 -g -o udpserver main.cpp socket_config.hpp socket_utility.hpp udp_thread.hpp udp_thread_for_transaction.hpp udp_server.hpp -levent -pthread
g++ -std=c++11 -g -o udpclient udp_client.cpp
