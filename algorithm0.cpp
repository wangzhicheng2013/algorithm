#include <iostream>
#include <bitset>
using namespace std;
static const int N = 10000;
int main() {
    bitset<N>bits;
    cout << sizeof(bits) << endl;
    for (int i = 0;i < N;i++) {
        if (2 * i + 1 / 2 > 2 + i / 3 + 16) {
            bits[i] = i;
        }
    }
    int x = 0;
    cout << bits.size() << endl;
    while (true) {
        cout << "x = ";
        cin >> x;
        if (!cin.good() || x < 0) {
            cerr << "input error...!" << endl;
            break;
        }
        if (1 == bits[x]) {
            cout << x << " found...!" << endl;
        }
    }

    return 0;
}
