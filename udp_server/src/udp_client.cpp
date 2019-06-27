#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
using namespace std;
int main() {
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    char buf[] = "hello server I am client...!";
    for (int i = 0;i < 10;i++) {
        sendto(sockfd, &buf, strlen(buf), 0, (struct sockaddr*)&addr,sizeof(addr));
        socklen_t len = sizeof(addr);
        char tmp[1024] = "";
        recvfrom(sockfd, tmp, sizeof(tmp), 0,(struct sockaddr*)&addr,&len);
        if (tmp[0])
        {
            cout << "client recv = " << tmp << endl;
        }
    }
    close(sockfd);
}
