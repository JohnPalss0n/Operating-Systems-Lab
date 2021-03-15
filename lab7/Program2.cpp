#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
using namespace std;

void sig_handler(int sign){
    static bool l = 0;
    if (!l){
	cout << "\nGet SIGPIPE. Press any key for exit.\n";
	l = 1;
    }
}

void* f2(void* arg);

struct str{
    int fd;
    pthread_t* id;
    bool fl;
};

void* f1(void* arg){
    cout << "f1 started.\n";
    while (((str*)arg)->fl == 0){
       if ((((str*)arg)->fd = open("/tmp/pipe", O_WRONLY | O_NONBLOCK)) <= 0){
           perror("open");
	   sleep(1);
       }
       else{
	   cout << "/tmp/pipe is opened.\n" ;
	   pthread_create(((str*)arg)->id, NULL, f2, arg);
	   cout << "f1 finished.\n";
    	   pthread_exit((int*)8);
       }
    }
    cout << "f1 finished.\n";
    pthread_exit((int*)8);
}

void* f2(void* arg){
    cout << "f2 started.\n";
    int buf = 0;
    while (((str*)arg)->fl == 0){
        buf++;
	write(((str*)arg)->fd, &buf, sizeof(buf));
	signal(SIGPIPE, sig_handler);
        sleep(1);
    }
    cout << "f2 finished.\n";
    pthread_exit((int*)9);
}

int main(){
    cout << "Program2 started.\n";
    pthread_t id1, id2;
    int e1, e2;
    str fif;
    fif.fl = 0;
    fif.id = &id2;

    if (mkfifo("/tmp/pipe", 0644) != 0)
	perror("mkfifo");
    else
	cout << "/tmp/pipe is created.\n";

    pthread_create(&id1, NULL, f1, &fif);
    cout << "Press key.\n";
    getchar();
    fif.fl = 1;
    pthread_join(id1, (void**)&e1);
    pthread_join(id2, (void**)&e2);
    cout << "Completion codes: " << e1 << ", " << e2 << endl;
    close(fif.fd);
    unlink("/tmp/pipe");
    cout << "Program2 finished.\n";
    return 0;
}
