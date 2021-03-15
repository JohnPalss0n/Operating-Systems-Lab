#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
using namespace std;

struct str{
    FILE* file;
    sem_t* sem; 
    bool fl;
};

void* f2(void* arg){
    cout << "f2 started.\n";
    char s = '2';
    while (((str*)arg)->fl == 0){
	sem_wait(((str*)arg)->sem);
	for (int i = 0; i < 5; ++i){
	    fputc(int(s), ((str*)arg)->file);
	    fflush(((str*)arg)->file);
            sleep(1);
	}
        sem_post(((str*)arg)->sem);
        sleep(1);
    }
    cout << "f2 end.\n";
    pthread_exit((int*)8);
}

int main(){
    cout << "Program22 started.\n";
    pthread_t id;
    int e;
    bool fl = 0;
    sem_t* sem;
    str fsf;
    fsf.sem = sem;
    fsf.fl = fl;
    fsf.sem = sem_open("/sem", O_CREAT, 0644, 1);
    fsf.file = fopen("test.txt","a+");
    if (fsf.file == NULL){
	cout << "Error\n";
    }
    pthread_create(&id, NULL, f2, &fsf);
    cout << "Press key.\n";
    getchar();
    fsf.fl = 1;
    pthread_join(id, (void**)&e);
    cout << "Completion codes: " << e << endl;
    fclose(fsf.file);
    sem_close(sem);
    sem_unlink("/sem");
    cout << "Program22 end.\n";
    return 0;
}
