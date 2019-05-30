#include <unistd.h>
#include "simple_timer.hpp"
void fun0() {
    cout << "I am fun0 run once...!" << endl;
}
void fun1() {
    cout << "I am fun1 run again...!" << endl;
}
int main() {
    simple_timer timer;
    cout << "start to timer..." << endl;
    timer.start_once(1000, fun0);
    cout << "end to timer..." << endl;

    cout << "start to timer..." << endl;
    timer.start(1000, fun1);
    sleep(5);
    timer.stop();
    cout << "end to timer..." << endl;

    return 0;
}
