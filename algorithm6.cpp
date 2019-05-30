#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
static const int N = 1000000;
int sum = 0;
int array[N] = {0};
void test0() {
    for (int i = 0;i < N;i++) {
        sum += array[i];
    }
}
void test1() {
    sum = accumulate(array, array + N, 0);
}
int main() {
    for (int i = 0;i < N;i++) {
        array[i] = i;
    }
//  test0();    // 0.018s ~0.020s
    test1();    // 0.019s ~0.020s

    return 0;
}
