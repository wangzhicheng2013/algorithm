#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
float fun(float x) {
    int i = *(int *)&x;
    i = 1597463007 - (i >> 1);
    x = *(float *)&i;
    return x;
}
int main() {
    int cnt = 0;
    for (float x = 10.098;x < 1000.890;x += 0.1) {
        float a = fun(x);
        float b = sqrt(1 / x);
        if (fabs(a - b) >= 0.001) {
            ++cnt;
            cout << "fun = " << a << endl;
            cout << "math = " << b << endl;
        }
    }
    cout << "cnt = " << cnt << endl;

    return 0;
}
