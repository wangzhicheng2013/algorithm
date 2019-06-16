#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
using namespace std;
//using tax_func = function<double(double)>;
typedef double (*tax_func)(double);
class tax_compute {
public:
    inline static tax_compute &get_instance() {
        return tax_compute_;
    }
public:
    double get_tax(double income) {
        if (income <= 0) {
            return 0;
        }
        int index = income / 100;
        if (index > tax5_range) {
            index = tax5_range;
        }
        return func_vec_[index](income);
    }
private:
    tax_compute() {
        func_vec_.resize(range);
        int i = 0;
        for (;i <= tax0_range;i++) {
            func_vec_[i] = get_tax0;
        }
        for (i = tax0_range + 1;i <= tax1_range;i++) {
            func_vec_[i] = get_tax1;
        }
        for (i = tax1_range + 1;i <= tax2_range;i++) {
            func_vec_[i] = get_tax2;
        }
        for (i = tax2_range + 1;i <= tax3_range;i++) {
            func_vec_[i] = get_tax3;
        }
        for (i = tax3_range + 1;i <= tax4_range;i++) {
            func_vec_[i] = get_tax4;
        }
        func_vec_[i] = get_tax5;

    }
    ~tax_compute() = default;
private:
    inline static double get_tax0(double income) {
        return 0;
    }
    inline static double get_tax1(double income) {
        return 0.2 * (income - 2200);
    }
    inline static double get_tax2(double income) {
        return 1.5 * (income - 2700) + 120;
    }
    inline static double get_tax3(double income) {
        return 2.2 * (income - 3200) + 520;
    }
    inline static double get_tax4(double income) {
        return 3.12 * (income - 3700) + 1220;
    }
    inline static double get_tax5(double income) {
        return 0.17 * income;
    }
private:
    static tax_compute tax_compute_;
private:
    const int range = 44;
    const int tax0_range = 22;
    const int tax1_range = 27;
    const int tax2_range = 32;
    const int tax3_range = 37;
    const int tax4_range = 42;
    const int tax5_range = 43;
    vector<tax_func>func_vec_;
};
tax_compute tax_compute::tax_compute_;
double get_tax(double income) {
    if (income <= 2200) {
        return 0;
    }
    else if (income <= 2700) {
        return 0.2 * (income - 2200);
    }
    else if (income <= 3200) {
        return 1.5 * (income - 2700) + 120;
    }
    else if (income <= 3700) {
        return 2.2 * (income - 3200) + 520;
    }
    else if (income <= 4200) {
        return 3.12 * (income - 3700) + 1220;
    }
    else {
        return 0.17 * income;
    }
}
int main() {
    double tax = 0;
    double income = 0;
    while (income < 10000000000) {
        //tax = get_tax(income);                                    // 1.556s
        tax = tax_compute::get_instance().get_tax(income);          // 4.019s
        income = income + 100;
    }

    return 0;
}
