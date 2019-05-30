#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int>v0{1, 2, 3};
    //vector<int>v1(v0);
    vector<int>v1 = v0;
    for (auto e : v1) {
        cout << e << endl;
    }
    v1[0] = 10;
    for (auto e : v0) {
        cout << e << endl;
    }


    return 0;
}
