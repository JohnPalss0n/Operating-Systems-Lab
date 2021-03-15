#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

struct str{
    bool fl1;
    bool fl2;
    int* sock;
    pthread_mutex_t* mutex;
    socklen_t* saddrlen;
    sockaddr_in* saddr;
    vector <string> msglist;
};

void* f1(void* arg){
    cout << "f1 started.\n";
    char rcvbuf[256];
    int recvcount;
    while (((str*)arg)->fl1 == 0){
	memset(rcvbuf, 0, sizeof(rcvbuf));
	recvcount = recvfrom((*((str*)arg)->sock), rcvbuf, sizeof(rcvbuf), 0, (sockaddr*)(((str*)arg)->saddr), ((str*)arg)->saddrlen);
	if (recvcount == -1) {
	    perror("recvfrom");
	    sleep(1);
	}else{
	    pthread_mutex_lock(((str*)arg)->mutex);
	    ((str*)arg)->msglist.push_back(string(rcvbuf));
	    pthread_mutex_unlock(((str*)arg)->mutex);
	}
    }
    cout << "f1 end.\n";
    pthread_exit((int*)8);
}

void* f2(void* arg){
    cout << "f2 started.\n";
    utsname u;
    int len;
    char sndbuf[256];
    char temp[256];
    int sentcount;
    string S;
    while (((str*)arg)->fl2 == 0){
        pthread_mutex_lock(((str*)arg)->mutex);
	if (!((str*)arg)->msglist.empty()){
	    S = ((str*)arg)->msglist.back();
	    ((str*)arg)->msglist.pop_back();
            strcpy(temp, S.c_str());
	    pthread_mutex_unlock(((str*)arg)->mutex);
	    uname(&u);
	    len = sprintf(sndbuf, "%s, %s", u.sysname, temp);
	    sentcount = sendto((*((str*)arg)->sock), sndbuf, len, 0, (sockaddr*)(((str*)arg)->saddr), *(((str*)arg)->saddrlen));
	    if (sentcount == -1) {
		perror("sendto");
	    }else{
	        cout << "sendto: OK!\n" << flush;
	    }
	}
	else{
	    pthread_mutex_unlock(((str*)arg)->mutex);
	    sleep(1);
	}
    }
    cout << "f2 end.\n";
    pthread_exit((int*)9);
}

int main(){
    cout << "Server is running.\n";
    pthread_t id1, id2;
    pthread_mutex_t m;
    pthread_mutex_init(&m, NULL);
    str str1;
    str1.fl1 = 0;
    str1.fl2 = 0;
    int e1, e2;
    int mysock, optval = 1;
    mysock = socket(AF_INET, SOCK_DGRAM, 0);
    int status = fcntl(mysock, F_GETFL);
    fcntl(mysock, F_SETFL, O_NONBLOCK | status);
    setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(8000);
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(mysock, (sockaddr*)&bindaddr, sizeof(bindaddr));
    sockaddr_in saddr;
    socklen_t saddrlen;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddrlen = sizeof(saddr);
    str1.saddr = &saddr;
    str1.saddrlen = &saddrlen;
    str1.sock = &mysock;
    str1.mutex = &m;
    pthread_create(&id1, NULL, f1, &str1);
    pthread_create(&id2, NULL, f2, &str1);
    cout << "Press key.\n";
    getchar();
    str1.fl1 = 1;
    str1.fl2 = 1;
    pthread_join(id1, (void**)&e1);
    pthread_join(id2, (void**)&e2);
    cout << "Completion codes: " << e1 << ", " << e2 << endl;
    pthread_mutex_destroy(&m);
    close(mysock);
    return 0;
}
