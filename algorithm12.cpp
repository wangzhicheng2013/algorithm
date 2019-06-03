#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
static const int LOOP = 1000000;
double fun(int x, int y) {
    double z = 0;
    if (0 == x) {
        z = sqrt(120.001);
    }
    else if (0 == y) {
        z = fabs(-0.00001);
    }
    else {
        z = pow(100, 3);
    }
    z = z + 1;
    return z;
}
double fun1(int x, int y) {
    double z = 0;
    if (x != 0 && y != 0) {
        z = pow(100, 3);
        z = z + 1;
        return z;
    }
    if (0 == x) {
        z = sqrt(120.001);
    }
    else {
        z = fabs(-0.00001);
    }
    z = z + 1;
    return z;
}
int main() {
    for (int i = 0;i < LOOP;i++) {
        int x = i;
        int y = i + 1;
        //double z = fun(x, y);      // 0.060~0.065s
        double z = fun1(x, y);       // 0.055~0.063s
    }

    return 0;
}
