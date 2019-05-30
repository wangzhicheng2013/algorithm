#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
class print_star {
public:
    print_star(int n) : n(n) {
    }
public:
    void operator () () {
        for (int i = 0;i < n;i++) {
            print0(i);
            print1(i);
            print2(i);
            print3(i);
        }
    }
private:
    void print0(int i) {
        for (int j = 0;j < n;j++) {
            i >= j ? cout << "*" : cout << " ";
        }
        cout << "\t";
    }
    void print1(int i) {
        for (int j = 0;j < n;j++) {
            i + j <= n - 1 ? cout << "*" : cout << " ";
        }
        cout << "\t";
    }
    void print2(int i) {
        for (int j = 0;j < n;j++) {
            i <= j ? cout << "*" : cout << " ";
        }
        cout << "\t";
    }
    void print3(int i) {
        for (int j = 0;j < n;j++) {
            i + j >= n - 1 ? cout << "*" : cout << " ";
        }
        cout << endl;
    }

public:
    int n;
};
int main() {
    print_star p(10);
    p();

    return 0;
}
