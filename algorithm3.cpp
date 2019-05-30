#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
class Test {
protected:
    ~Test() {
        cout << "~Test()" << endl;
    }

};
int main() {
    Test *p = new Test;
    delete p;

    return 0;
}
