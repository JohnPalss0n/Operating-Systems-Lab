#include <pthread.h>
#include <string.h>
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

void* f2(void* arg){
    cout << "f2 started.\n";
    int result;
    TMessage message; 
    message.mtype = 1;
    while (((str*)arg)->fl == 0){
	memset(message.buff, 0, sizeof(message.buff));
	result = msgrcv(((str*)arg)->msgid, &message, sizeof(message.buff), message.mtype, IPC_NOWAIT);
	if (result > 0)
	    cout << "msgrcv: result = " << result << ' ' << message.buff << endl;
	else{
	    perror("msgrcv");
	    sleep(1);
	}
    }
    cout << "f2 finished.\n";
    pthread_exit((int*)9);
}

int main(){
    cout << "Program2 started.\n";
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
    pthread_create(&id, NULL, f2, &fms);
    cout << "Press key.\n";
    getchar();
    fms.fl = 1;
    pthread_join(id, (void**)&e);
    cout << "Completion code: " << e << endl;
    msgctl(fms.msgid, IPC_RMID, NULL);
    cout << "Program2 finished.\n";
    return 0;
}
