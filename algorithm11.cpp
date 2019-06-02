#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
static const int LOOP = 1000000;
char get_grade0(unsigned char score) {
    if (0 <= score && score <= 100) {
            if (90 <= score && score <= 100) {
                return 'A';
            }
            else if (80 <= score && score < 90) {
                return 'B';
            }
            else if (70 <= score && score < 80) {
                return 'C';
            }
            else if (60 <= score && score < 70) {
                return 'D';
            }
            else {
                return 'E';
            }
    }
    return 'F';
}
char get_grade1(unsigned char score) {
    if (score < 0 || score > 100) {
        return 'F';
    }
    switch (score / 10) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 'E';
    case 6:
        return 'D';
    case 7:
        return 'C';
    case 8:
        return 'B';
    case 9:
        return 'A';
    }
    return 'A';
}
static const unsigned char N = 101;
char score_array[N] = {0};
void init_score_array() {
    unsigned char score = 0;
    for (;score < N;++score) {
        if (90 <= score && score <= 100) {
            score_array[score] = 'A';
            continue;
        }
        if (80 <= score && score < 90) {
            score_array[score] = 'B';
            continue;
        }
        if (70 <= score && score < 80) {
            score_array[score] = 'C';
            continue;
        }
        if (60 <= score && score < 70) {
            score_array[score] = 'D';
            continue;
        }
        score_array[score] = 'E';
    }
}
inline char get_grade2(unsigned char score) {
    return score_array[score];
}
int main() {
    init_score_array();
    cout << get_grade2(100) << endl;
    for (int i = 0;i < LOOP;i++) {
        for (unsigned char score = 0;score <= 100;++score) {
            //char grade = get_grade0(score);         // 0.867~0.869s
            //char grade = get_grade1(score);         // 0.995~0.996s
            char grade = get_grade2(score);           // 0.502~0.505s
        }
    }

    return 0;
}
