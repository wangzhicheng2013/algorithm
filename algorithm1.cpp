#include <iostream>
#include <string>
using namespace std;
static const int LOOP = 1000000;
void string_cat(string &str, const string &src_str, size_t begin_pos, size_t end_pos) {
    for (size_t i = begin_pos;i < end_pos;i++) {
        str += src_str[i];
    }
}
void left_rotate(string &str, size_t pos) {
    string tmp;
    string_cat(tmp, str, pos, str.size());
    string_cat(tmp, str, 0, pos);
    str = tmp;
}
void left_rotate1(string &str, size_t pos) {
    string tmp;
    tmp.assign(str, 0, pos);
    size_t size = str.size();
    for (size_t i = pos;i < size;i++) {
        str[i - pos] = str[i];
    }
    size_t i = 0;
    for (size_t start = size - pos;start < size;start++) {
        str[start] = tmp[i++];
    }
    
}
int main() {
    for (int i = 0;i < LOOP;i++) {
        string str = "abcdef";
        // left_rotate(str, 2);     // 0.626s
        left_rotate1(str, 2);       // 0.324~0.325s
    }

    return 0;
}
