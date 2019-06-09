#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <utime.h>
#include <iostream>
#include <fstream>
using namespace std;
const char *file_path = nullptr;
inline bool need_to_create_file() {
    if (0 == access(file_path, R_OK)) {
        return false;
    }
    ofstream ofs(file_path, ios::in | ios::app);
    if (!ofs) {
        cerr << file_path << " create failed...!" << endl;
    }
    else {
        cout << file_path << " create ok." << endl;
        ofs.close();
    }
    return true;
}
inline void update_file() {
    utimbuf utim_buf = {time(NULL), time(NULL)};
    if (utime(file_path, &utim_buf) < 0) {
        cerr << strerror(errno) << endl;
    }
    else {
        cout << "update " << file_path << " access time ok." << endl;
    }
}
int main(int argc, const char **argv) {
    if (argc != 2) {
        cerr << "please input file path." << endl;
        return -1;
    }
    file_path = argv[1];
    if (true == need_to_create_file()) {
        return 0;
    }
    update_file();

    return 0;
}
