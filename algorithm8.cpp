#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
const char *str = "AAA AAA     AAAABBB";
int fun0(string &res) {
    int len = strlen(str);
    int cnt = 0;
    for (int i = 0;i < len;i++) {
        if (32 == str[i]) {
            cnt++;
            continue;
        }
        res += str[i];
    }
    return cnt;
}
int fun1(char *res) {
    int len = strlen(str);
    int cnt = 0;
    for (int i = 0;i < len;i++) {
        if (32 == str[i]) {
            cnt++;
            continue;
        }
        res[i] = str[i];
    }
    return cnt;
}
int main() {
//  string res;
//  cout << fun0(res) << endl;  // 0.004s
    char *res = (char *)malloc(strlen(str) + 1);   // 0.004 
    cout << fun1(res) << endl;
    free(res);

    return 0;
}
