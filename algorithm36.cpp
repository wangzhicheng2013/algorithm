#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <chrono>
#include <thread>
#include <type_traits>
#include <stdexcept>
using namespace std;
template <typename T>
class lock_free_queue {
    static_assert(is_move_constructible<T>::value, "should be a movable type");
public:
    lock_free_queue(size_t size = 1024) : buffer_(size), buffer_mask_(size - 1), enqueue_pos_(0), dequeue_pos_(0) {
        check_size(size);
        for (auto i = 0;i < size;i++) {
            buffer_[i].sequence = i;
        }
    }
    inline lock_free_queue(const lock_free_queue &&rhs) noexcept {
        if (this != &rhs) {
            *this = rhs;
        }
    }
    inline lock_free_queue & operator = (const lock_free_queue &&rhs) noexcept {
        if (this == &rhs) {
            return;
        }
        buffer_ = move(rhs.buffer_);
        buffer_mask_ = rhs.buffer_mask_;
        enqueue_pos_ = rhs.enqueue_pos_.load();
        dequeue_pos_ = rhs.dequeue_pos_.load();
        return *this;
    }
    bool push(const T &data) {
        cell *cell_ptr = nullptr;
        size_t pos = enqueue_pos_.load(memory_order_relaxed);
        while (true) {
            cell_ptr = &buffer_[pos & buffer_mask_];
            size_t sequence = cell_ptr->sequence.load(memory_order_acquire);
            intptr_t diff = (intptr_t)sequence - (intptr_t)pos; 
            if (0 == diff) {
                if (enqueue_pos_.compare_exchange_weak(pos, pos + 1, memory_order_relaxed)) {
                    break;
                }
                continue;
            }
            else if (diff < 0) {
                return false;
            }
            pos = enqueue_pos_.load(memory_order_relaxed);
        }
        cell_ptr->data = data;
        cell_ptr->sequence.store(pos + 1, memory_order_release);
        return true;
    }
    bool pop(T &data) {
        cell *cell_ptr = nullptr;
        size_t pos = dequeue_pos_.load(memory_order_relaxed);
        while (true) {
            cell_ptr = &buffer_[pos & buffer_mask_];
            size_t sequence = cell_ptr->sequence.load(memory_order_acquire);
            intptr_t diff = (intptr_t)sequence - (intptr_t)(pos + 1); 
            if (0 == diff) {
                if (dequeue_pos_.compare_exchange_weak(pos, pos + 1, memory_order_relaxed)) {
                    break;
                }
                continue;
            }
            else if (diff < 0) {
                return false;
            }
            pos = dequeue_pos_.load(memory_order_relaxed);
        }
        data = move(cell_ptr->data);
        cell_ptr->sequence.store(pos + buffer_mask_ + 1, memory_order_release);
        return true;
    }
    inline bool empty() {
        return 0 == (enqueue_pos_.load(memory_order_relaxed) - dequeue_pos_.load(memory_order_relaxed));
    }
    inline void set_capacity(size_t cap) {
        max_capacity = cap;
    }
public:
    inline static bool number_is_power_of_two(size_t num) {
        return (num >= 2) && (0 == (num & (num - 1)));
    }
private:
    void check_size(size_t &size) {
        if (size <= 0 || size > max_capacity) {
            size = max_capacity;
        }
        if (true == number_is_power_of_two(size)) {
            return;
        }
        while (size < max_capacity) {
            if (true == number_is_power_of_two(++size)) {
                break;
            }
        }
    }
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
    size_t max_capacity = 1024 * 8;
private:
    typedef char cache_line[64];
    cache_line pad0_;
    vector<cell>buffer_;
    size_t buffer_mask_;
    cache_line pad1_;
    atomic<size_t>enqueue_pos_;
    cache_line pad2_;
    atomic<size_t>dequeue_pos_;
    cache_line pad3_;
};
static const int N = 1000000;
static string str(1024, 'A');
lock_free_queue<string>G_Q;
void put_e() {
    for (int i = 0;i < N;i++) {
        G_Q.push(str);
    }
}
void get_e() {
    int n = 0;
    string str;
    this_thread::sleep_for(chrono::microseconds(1));
    while (n < N) {
        n++;
        G_Q.pop(str);
    }
    //cout << n << endl; // 1000000
}
int main() {
    G_Q.set_capacity(1024 * 1024);
    thread th0(put_e);
    thread th1(get_e);
    th0.join();
    th1.join();
    // throughput 8264462.809917356 string/s
    return 0;
}

