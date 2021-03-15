#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

struct str{
    bool fl;
    int* sock;
    socklen_t* saddrlen;
    sockaddr_in* saddr;
};

void* f1(void* arg){
    cout << "f1 started.\n";
    int len;
    char sndbuf[256];
    int sentcount, count = 0;
    while (((str*)arg)->fl == 0){
        len = sprintf(sndbuf, "request %d", count);
	sentcount = sendto((*((str*)arg)->sock), sndbuf, len, 0, (sockaddr*)(((str*)arg)->saddr), *(((str*)arg)->saddrlen));
	if (sentcount == -1) {
	    perror("sendto");
	}else{
	    cout << "sendto: OK!\n" << flush;
	}
	count++;
	sleep(1);
    }
    cout << "f1 end.\n";
    pthread_exit((int*)8);
}

void* f2(void* arg){
    cout << "f2 started.\n";
    char rcvbuf[256];
    int recvcount, count = 0;
    while (((str*)arg)->fl == 0){
	memset(rcvbuf,0,sizeof(rcvbuf));
	recvcount = recvfrom((*((str*)arg)->sock), rcvbuf, sizeof(rcvbuf), 0, (sockaddr*)(((str*)arg)->saddr), (((str*)arg)->saddrlen));
	if (recvcount == -1) {
	    perror("recvfrom");
	    sleep(1);
	}else{
	    printf("%s\n", rcvbuf);
	}
	count++;
	sleep(1);
    }
    cout << "f2 end.\n";
    pthread_exit((int*)9);
}

int main(){
    cout << "Client is running.\n";
    pthread_t id1, id2;
    str str1, str2;
    str1.fl = 0;
    str2.fl = 0;
    int e1, e2;
    int mysock, optval = 1;
    mysock = socket(AF_INET, SOCK_DGRAM, 0);
    int status = fcntl(mysock, F_GETFL);
    fcntl(mysock, F_SETFL, O_NONBLOCK | status);
    setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(7000);
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(mysock, (sockaddr*)&bindaddr, sizeof(bindaddr));
    sockaddr_in saddr;
    socklen_t saddrlen;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddrlen = sizeof(saddr);
    str1.saddr = &saddr;
    str1.saddrlen = &saddrlen;
    str1.sock = &mysock;
    str2.saddr = &saddr;
    str2.saddrlen = &saddrlen;
    str2.sock = &mysock;
    pthread_create(&id1, NULL, f1, &str1);
    pthread_create(&id2, NULL, f2, &str2);
    cout << "Press key.\n";
    getchar();
    str1.fl = 1;
    str2.fl = 1;
    pthread_join(id1, (void**)&e1);
    pthread_join(id2, (void**)&e2);
    cout << "Completion codes: " << e1 << ", " << e2 << endl;
    close(mysock);
    return 0;
}
