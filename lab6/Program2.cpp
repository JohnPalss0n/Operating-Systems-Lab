#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

struct str{
    char* addr;
    sem_t* semread; 
    sem_t* semwrite; 
    bool fl;
};

void* f2(void* arg){
    cout << "f2 started.\n";
    int value;
    while (((str*)arg)->fl == 0){
	sem_wait(((str*)arg)->semwrite);
        memcpy(&value, (int*)((str*)arg)->addr, sizeof(int));
	cout << value << endl;
        sem_post(((str*)arg)->semread);
    }
    cout << "f2 finished.\n";
    pthread_exit((int*)8);
}

int main(){
    cout << "Program2 started.\n";
    pthread_t id;
    int e, mem_id;
    bool fl = 0;
    sem_t* semread;
    sem_t* semwrite;
    char* ptr;
    key_t mykey;
    mykey = ftok("file", 0);
    mem_id = shmget(mykey, 4096, IPC_CREAT|0666);
    ptr = (char*)shmat(mem_id, NULL, 0);
    str buf;
    buf.addr = ptr;
    buf.semread = semread;
    buf.semwrite = semwrite;
    buf.fl = fl;
    buf.semread = sem_open("/semread", O_CREAT, 0644, 0);  
    buf.semwrite = sem_open("/semwrite", O_CREAT, 0644, 0);
    pthread_create(&id, NULL, f2, &buf);
    cout << "Press key.\n";
    getchar();
    buf.fl = 1;
    pthread_join(id, (void**)&e);
    cout << "Completion codes: " << e << endl;
    sem_close(semread);
    sem_unlink("/semread");
    sem_close(semwrite);
    sem_unlink("/semwrite");
    shmdt(ptr);
    shmctl(mem_id, IPC_RMID, (shmid_ds*)ptr);
    cout << "Program2 finished.\n";
    return 0;
}
