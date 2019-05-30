#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
auto add_fun = [](auto x, auto y) { return x + y;};
int main() {
    cout << add_fun(1, 100) << endl;

    return 0;
}
