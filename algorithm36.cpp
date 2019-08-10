#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <type_traits>
#include <stdexcept>
using namespace std;
template <typename T>
class lock_free_queue {
    static_assert(is_move_constructible<T>::value, "should be a movable type");
private:
    struct cell {
        cell() = default;
        cell(const cell &) = delete;
        cell & operator = (const cell &) = delete;
        cell(cell &&rhs) : data(move(rhs.data)), sequence(rhs.sequence.load()) {
        }
        cell & operator = (cell &&rhs) {
            data(move(rhs.data));
            sequence(rhs.sequence.load());
            return *this;
        }
        T data;
        atomic<size_t>sequence;

    };
private:
    typedef char cache_line[64];
    cache_line pad0_;
    vector<cell>buffer_;
    cache_line pad1_;
    atomic<size_t>enqueue_pos;
    cache_line pad2_;
    atomic<size_t>dequeue_pos;
    cache_line pad3_;
};
//https://github.com/wangzhicheng2013/robot/blob/master/zeg_robot_host_communication/include/thread_pool/mpmc_bounded_queue.hpp
int main() {
    lock_free_queue<int>qq;

    return 0;
}

