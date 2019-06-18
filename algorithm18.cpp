#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <zmq.h>
using namespace std;
int do_router(void* router, void* dealer) {
    while (true) {
        int more = 1;
        zmq_msg_t message;
        zmq_msg_init(&message);
        int rc = zmq_msg_recv(&message, router, 0);
        size_t more_size = sizeof(more);
        zmq_getsockopt(router, ZMQ_RCVMORE, &more, &more_size);
        zmq_msg_send(&message, dealer, more ? ZMQ_SNDMORE : 0);
        printf("[%ld] router deliver request to dealer. rc = %d, more = %d\n", time(NULL), rc, more);
        zmq_msg_close(&message);
        if (!more) {
            break; // Last message part
        }
    }
    printf("[%ld]----------DoRouter----------\n\n", time(NULL));
    return 0;
}
int do_realer(void* router, void* dealer) {
    while (true) {
        int more = 1;
        zmq_msg_t message;
        zmq_msg_init(&message);
        int rc = zmq_msg_recv(&message, dealer, 0);
        size_t more_size = sizeof (more);
        zmq_getsockopt(dealer, ZMQ_RCVMORE, &more, &more_size);
        zmq_msg_send(&message, router, more ? ZMQ_SNDMORE : 0);
        printf("[%ld] dealer deliver reply to router. rc = %d, more = %d\n", time(NULL), rc, more);
        zmq_msg_close(&message);
        if (!more) {
            break; // Last message part
        }
    }
    printf("[%ld]----------DoDealer----------\n\n", time(NULL));
    return 0;
}
void work0() {
    void* context = zmq_ctx_new();
    void* worker = zmq_socket(context, ZMQ_REP);
    const char *addr = "tcp://127.0.0.1:9999";
    int rec = zmq_connect(worker, addr);
    if (rec) {
        printf("zmq_connect %s error:%d\n", addr, rec);
        zmq_close(worker);
        zmq_ctx_term(context);
        return;
    }
    printf("worker zmq_connect %s done!\n", addr);
    char buf[128] = {0};
    while (true) {
        rec = zmq_recv(worker, buf, sizeof(buf), 0);
        printf("[%ld] recv request(%s) from client, rec = %d, request.len = %d\n", time(NULL), buf, rec, strlen(buf));
        rec = zmq_send(worker, buf, strlen(buf), 0);
        printf("[%ld] send reply(%s) to client, rec = %d, reply.len = %d\n", time(NULL), buf, rec, strlen(buf));
        printf("[%ld]------------------------\n\n", time(NULL));
    }
    zmq_close(worker);
    zmq_ctx_term(context);
    printf("good bye and good luck!\n");
}
void client() {
    void* context = zmq_ctx_new();
    void* client = zmq_socket(context, ZMQ_REQ);
    const char *addr = "tcp://127.0.0.1:8888";
    int rec = zmq_connect(client, addr);
    if (rec) {
        printf("zmq_connect %s error:%d\n", addr, rec);
        zmq_close(client);
        zmq_ctx_term(context);
        return;
    }
    printf("worker zmq_connect %s done!\n", addr);
    char buf[128] = "hello I am a client.";
    char buf1[128] = "";
    while (true) {
        rec = zmq_send(client, buf, strlen(buf), 0);
        printf("[%ld] send request(%s) from client, rec = %d, request.len = %d\n", time(NULL), buf, rec, strlen(buf));
        sleep(2);
        rec = zmq_recv(client, buf1, sizeof(buf1), 0);
        printf("[%ld] recv reply(%s) from server, rec = %d, reply.len = %d\n", time(NULL), buf, rec, strlen(buf));
        printf("[%ld]------------------------\n\n", time(NULL));
    }
    zmq_close(client);
    zmq_ctx_term(context);
    printf("good bye and good luck!\n");
}
int main(int argc, char** argv) {
    if (argc < 5) {
        printf("usage : %s router_ip router_port dealer_ip dealer_port\n", argv[0]);
        return -1;
    }
    int major, minor, patch;
    zmq_version (&major, &minor, &patch);
    printf ("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
    printf("===========================================\n\n");
    char addr[128] = {0};
    void* context = zmq_ctx_new();
    snprintf(addr, sizeof(addr), "tcp://%s:%s", argv[1], argv[2]);
    void* router = zmq_socket(context, ZMQ_ROUTER);
    int rc = zmq_bind(router, addr);
    printf("[%ld] router bind %s %s.\n", time(NULL), addr, (rc ? "error" : "ok"));
    if (rc) {
        printf("[%ld] router bind error : %s\n", time(NULL), zmq_strerror(zmq_errno()));
        zmq_close(router);
        zmq_ctx_term(context);
        return -2;
    }
    snprintf(addr, sizeof(addr), "tcp://%s:%s", argv[3], argv[4]);
    void* dealer = zmq_socket(context, ZMQ_DEALER);
    rc = zmq_bind(dealer, addr);
    printf("[%ld] dealer bind %s %s.\n", time(NULL), addr, (rc ? "error" : "ok"));
    if (rc) {
        printf("[%ld] dealer bind error : %s\n", time(NULL), zmq_strerror(zmq_errno()));
        zmq_close(router);
        zmq_close(dealer);
        zmq_ctx_term(context);
        return -3;
    }
    thread th0(work0);
    thread th1(client);
    zmq_pollitem_t items [] = { 
        {router, 0, ZMQ_POLLIN, 0} , {dealer, 0, ZMQ_POLLIN, 0} };
    while (true) {
        rc = zmq_poll(items, sizeof(items) / sizeof(zmq_pollitem_t), 1000);
        if (rc < 0) {
            printf("[%ld] zmq_poll error: %d\n", time(NULL), rc);
            break;
        }
        printf("[%ld] zmq_poll rc = %d\n", time(NULL), rc);
        if (rc < 1) {
            continue;
        }
        if (items[0].revents & ZMQ_POLLIN) {
            /* router可读事件，说明有client请求过来了。 */
            printf("[%ld] zmq_poll catch one router event!\n", time(NULL));
            do_router(router, dealer);
        }
        else if (items[1].revents & ZMQ_POLLIN) {
            /* dealer可读事件，说明有worker的应答数据到来了。 */
            printf("[%ld] zmq_poll catch one dealer event!\n", time(NULL));
            do_realer(router, dealer);
        }
        else {
            printf("[%ld] zmq_poll Don't Care this evnet!\n", time(NULL));
        }
    }
    th0.join();
    th1.join();
    zmq_close(router);
    zmq_close(dealer);
    zmq_ctx_term(context);
    printf("[%ld] good bye and good luck!\n", time(NULL));
    return 0;
}
