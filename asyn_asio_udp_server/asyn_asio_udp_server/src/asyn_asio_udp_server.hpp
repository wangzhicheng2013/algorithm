#ifndef ASYN_ASIO_UDP_SERVER_HPP_
#define ASYN_ASIO_UDP_SERVER_HPP_
#include <string.h>
#include <iostream>
#include <string>
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/noncopyable.hpp"
using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;
class asyn_asio_udp_server : public boost::noncopyable {
public:
	asyn_asio_udp_server() : sock_(s_ios, ip::udp::endpoint(ip::udp::v4(), 9000)) {
	}
	virtual ~asyn_asio_udp_server() = default;
public:
	void start() {
		do_read();
		s_ios.run();
	}
private:
	void on_read(const boost::system::error_code &err, size_t bytes) {
		if (!err) {
			string msg(read_buf_, bytes);
			do_write(msg);
		}
	}
	void do_read() {
		sock_.async_receive_from(buffer(read_buf_), sender_end_point_, boost::bind(&asyn_asio_udp_server::on_read, this, _1, _2));
	}
	void on_write(const boost::system::error_code &err, size_t bytes) {
		do_read();
	}
	void do_write(const string &msg) {
		copy(begin(msg), end(msg), write_buf_);
		sock_.async_send_to(buffer(write_buf_, msg.size()), sender_end_point_, boost::bind(&asyn_asio_udp_server::on_write, this, _1, _2));
	}
private:
	static const size_t s_buff_size = 1024;
private:
	char write_buf_[s_buff_size];
	char read_buf_[s_buff_size];
	ip::udp::endpoint sender_end_point_;
	ip::udp::socket sock_;
public:
	static io_service s_ios;
};
io_service asyn_asio_udp_server::s_ios;
#endif /* ASYN_ASIO_UDP_SERVER_HPP_ */
