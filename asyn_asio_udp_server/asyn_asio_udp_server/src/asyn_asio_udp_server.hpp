#ifndef ASYN_ASIO_UDP_SERVER_HPP_
#define ASYN_ASIO_UDP_SERVER_HPP_
#include <string.h>
#include <iostream>
#include <string>
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/noncopyable.hpp"
using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;
#define MEM_FN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x, y) boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x, y, z) boost::bind(&self_type::x, shared_from_this(), y, z)
class asyn_asio_udp_server : public boost::noncopyable, boost::enable_shared_from_this<asyn_asio_udp_server> {
public:
	using self_type = asyn_asio_udp_server;
	asyn_asio_udp_server(const string &msg) : sock_(s_ios, ip::udp::endpoint(ip::udp::v4(), 0)),
	message_(msg), start_(true){
	}
	virtual ~asyn_asio_udp_server() {
	}
public:
	inline bool started() const {
		return start_;
	}
	/*
	static auto start(const string &msg) {
		(new asyn_asio_udp_server(msg))->start();
	}*/
private:
	void on_read(const boost::system::error_code &err, size_t bytes) {
		if (!err) {
			if (0 == message_.compare(0, bytes, read_buf_)) {
				cout << "server echo message = " << message_ << endl;
			}
		}
	}
	void do_read() {
//		sock_.async_receive_from(buffer(read_buf_), sender_end_point_, MEM_FN2(on_read, _1, _2));
	}
	void on_write(const boost::system::error_code &err, size_t bytes) {
		do_read();
	}
	void do_write(const string &msg) {
		copy(begin(msg), end(msg), write_buf_);
		sock_.async_send_to(buffer(write_buf_, msg.size()), sender_end_point_, MEM_FN2(on_write, _1, _2));
	}
private:
	static const size_t s_buff_size = 1024;
private:
	ip::udp::socket sock_;
	ip::udp::endpoint sender_end_point_;
	char read_buf_[s_buff_size];
	char write_buf_[s_buff_size];
	bool start_;
	string message_;
public:
	static io_service s_ios;
};
io_service asyn_asio_udp_server::s_ios;
#endif /* ASYN_ASIO_UDP_SERVER_HPP_ */
