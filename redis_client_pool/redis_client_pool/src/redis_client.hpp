#ifndef REDIS_CLIENT_HPP_
#define REDIS_CLIENT_HPP_
#include <hiredis/hiredis.h>
#include <string.h>
#include <iostream>
#include <string>
using namespace std;
class redis_client {
public:
	redis_client() {
		conn_ = nullptr;
	}
	redis_client(const redis_client &) = delete;
	redis_client & operator = (const redis_client &) = delete;
	virtual ~redis_client() {
		if (nullptr != conn_) {
			redisFree(conn_);
			conn_ = nullptr;
		}
	}
	inline bool connect(const char *ip, int port) {
		conn_ = redisConnect(ip, port);
		if (nullptr == conn_) {
			cerr << "connection is null...!" << endl;
			return false;
		}
		if (conn_->err) {
			cerr << "connection failed err = " << conn_->errstr << endl;
			return false;
		}
		return true;
	}
	bool set_value(const char *key, const string &value) {
		if (nullptr == conn_) {
			cerr << "redis client conn is null...!" << endl;
			return false;
		}
		if (value.size() > MAXVALUESIZE) {
			cerr << "value size is over 1M...!" << endl;
			return false;
		}
		size_t keylen = strlen(key);
		if (keylen > MAXKEYSIZE) {
			cerr << "key size is over 1k...!" << endl;
			return false;
		}
		redisReply *reply = (redisReply *)redisCommand(conn_, "set %b %b", key, keylen, value.c_str(), value.size());
		if (nullptr == reply) {
			cerr << "reply is null...!" << endl;
			return false;
		}
		bool succ = true;
		if (nullptr == reply->str) {
			succ = false;
			cerr << "reply string is null...!" << endl;
		}
		else if (strcasecmp(reply->str, "ok")) {
			succ = false;
			cerr << "reply error = " << reply->str << endl;
		}
		freeReplyObject(reply);
		reply = nullptr;
		return succ;
	}
	bool get_value(const char *key, string &value) {
		if (nullptr == conn_) {
			cerr << "redis client conn is null...!" << endl;
			return false;
		}
		size_t keylen = strlen(key);
		if (keylen > MAXKEYSIZE) {
			cerr << "key size is over 1k...!" << endl;
			return false;
		}
		redisReply *reply = (redisReply *)redisCommand(conn_, "get %s", key);
		if (nullptr == reply) {
			cerr << "reply is null...!" << endl;
			return false;
		}
		bool succ = true;
		if (nullptr == reply->str) {
			succ = false;
			cerr << "reply string is null...!" << endl;
		}
		else if (REDIS_REPLY_NIL == reply->type) {
			succ = false;
			cerr << "reply error = " << reply->str << endl;
		}
		else {
			value.assign(reply->str, reply->len);
		}
		freeReplyObject(reply);
		reply = nullptr;
		return succ;
	}
private:
	redisContext *conn_;
private:
	static const size_t MAXVALUESIZE = 1024 * 1024;
	static const size_t MAXKEYSIZE = 1024;
	static const size_t MAXCHANNELSIZE = 10;
};

#endif /* REDIS_CLIENT_HPP_ */
