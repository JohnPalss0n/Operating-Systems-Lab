#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
using namespace std;

struct str{
    FILE* file;
    bool fl;
};

void* f2(void* arg){
    cout << "f2 started.\n";
    char s = '2';
    while (((str*)arg)->fl == 0){
        fputc(int(s), ((str*)arg)->file);
	fflush(((str*)arg)->file);
        sleep(1);
    }
    cout << "f2 end.\n";
    pthread_exit((int*)8);
}

int main(){
    cout << "Program12 started.\n";
    pthread_t id;
    int e;
    bool fl = 0;
    str fsf;
    fsf.fl = fl;
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
    cout << "Program12 end.\n";
    return 0;
}
