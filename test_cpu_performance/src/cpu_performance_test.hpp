#ifndef CPU_PERFORMANCE_TEST_HPP_
#define CPU_PERFORMANCE_TEST_HPP_
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
using namespace std;
struct coordinate {
	coordinate(int a = 0, int b = 0) : row(a), col(b) {
	}
	int row;
	int col;
};
class cpu_performance_test {
public:
	static inline cpu_performance_test &get_instance() {
		static cpu_performance_test test;
		return test;
	}
	inline void set_matrix_dimension(int d) {
		matrix_dimension_ = d;
	}
	void init() {
		init_square_matrix(square_matrix0_);
		init_square_matrix(square_matrix1_);
		init_coordinate_array();
		init_threads();
	}
private:
	cpu_performance_test() {
		thread_num_ = thread::hardware_concurrency();
		matrix_dimension_ = 100;
	}
	cpu_performance_test(const cpu_performance_test &) = delete;
	cpu_performance_test & operator = (const cpu_performance_test &) = delete;
	virtual ~cpu_performance_test() {
		join_threads();
	}
private:
	void init_square_matrix(vector<vector<int> >&square_matrix) {
		int number = 0;
		vector<int>matrix_line;
		random_device rd;
		matrix_line.reserve(matrix_dimension_);
		cout << "square matrix as following:" << endl;
		cout << "===============================================" << endl;
		for (int i = 0;i < matrix_dimension_;i++) {
			for (int j = 0;j < matrix_dimension_;j++) {
				number = rd() % 1888;
				matrix_line.emplace_back(number);
				cout << number << " ";
			}
			square_matrix.emplace_back(matrix_line);
			matrix_line.clear();
			cout << endl;
		}
		cout << "===============================================" << endl;
	}
	void init_coordinate_array() {
		corrdinate_array_.resize(thread_num_);
		int loop = 0;
		for (int i = 0;i < matrix_dimension_;i++) {
			for (int j = 0;j < matrix_dimension_;j++) {
				corrdinate_array_[loop].emplace_back(coordinate(i, j));
				cout << "thread id = " << loop << endl;
				cout << "thread task data is " << "(" << i << "," << j << ")" << endl;
				loop = (loop + 1) % thread_num_;

			}
		}
	}
	void init_threads() {
		cout << "thread number = " << thread_num_ << endl;
		threads_.resize(thread_num_);
		int thread_id = 0;
		now_ = std::chrono::steady_clock::now();
		for (auto &thread_obj : threads_) {
			thread_obj = thread(bind(&cpu_performance_test::thread_compute, this, thread_id++));
		}
	}
	void join_threads() {
		for (auto &thread_obj : threads_) {
			if (thread_obj.joinable()) {
				thread_obj.join();
			}
		}
		auto end_time = chrono::steady_clock::now();
		chrono::duration<double>time_span = chrono::duration_cast<chrono::duration<double>>(end_time - now_);
		cout << "elapse time = " << time_span.count() << " seconds." << endl;
	}
	void thread_compute(int thread_id) {
		const auto vec = corrdinate_array_[thread_id];
		for (auto &coordinate_obj : vec) {
			for (int i = 0;i < matrix_dimension_;i++) {
				square_matrix0_[coordinate_obj.row][i] * square_matrix1_[i][coordinate_obj.col];
			}
		}
	}
private:
	int thread_num_;
	int matrix_dimension_;
private:
	vector<vector<int> >square_matrix0_;
	vector<vector<int> >square_matrix1_;
	vector<vector<coordinate>>corrdinate_array_;
	vector<thread>threads_;
private:
	chrono::steady_clock::time_point now_;
};

#endif /* CPU_PERFORMANCE_TEST_HPP_ */
