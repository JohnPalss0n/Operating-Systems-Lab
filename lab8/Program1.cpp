#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
using namespace std;

struct TMessage{
    long mtype;
    char buff[256];
};

struct str{
    bool fl;
    int msgid;
};

void* f1(void* arg){
    cout << "f1 started.\n";
    int len, count = 0, result;
    TMessage message; 
    message.mtype = 1;
    while (((str*)arg)->fl == 0){
	count++;
        len = sprintf(message.buff, "Hello, %d", count);
	result = msgsnd(((str*)arg)->msgid, &message, len, IPC_NOWAIT);
	if (result == -1)
	    perror("msgsnd");
	else
	    cout << "OK!\n" << flush;
	sleep(1);
    }
    cout << "f1 finished.\n";
    pthread_exit((int*)8);
}

int main(){
    cout << "Program1 started.\n";
    pthread_t id;
    int e;
    str fms;
    fms.fl = 0;
    key_t key;
    key = ftok("file", 'A');
    fms.msgid = msgget(key, 0);
    if (fms.msgid < 0){
	perror("msgid");
	fms.msgid = msgget(key, IPC_CREAT | 0666);
    }
    pthread_create(&id, NULL, f1, &fms);
    cout << "Press key.\n";
    getchar();
    fms.fl = 1;
    pthread_join(id, (void**)&e);
    cout << "Completion code: " << e << endl;
    msgctl(fms.msgid, IPC_RMID, NULL);
    cout << "Program1 finished.\n";
    return 0;
}
