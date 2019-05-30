#include "boost/coroutine2/all.hpp"
#include <iostream>
#include <string>
using namespace std;
using boost::coroutines2::coroutine;
int main() {
    typedef boost::coroutines2::coroutine<int> coro_t2;
    coro_t2::pull_type source([&](coro_t2::push_type &sink) {
                    sink(1);
                    sink(10);
                    sink(100);
                });
    return 0;
}
