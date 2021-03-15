#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
using namespace std;

struct str{
    int fd;
    bool fl;
};

void* f1(void* arg){
    cout << "f1 started.\n";
    int rc, buf;
    while (((str*)arg)->fl == 0){
        buf = 0;
	rc = read(((str*)arg)->fd, &buf, sizeof(buf));
	if (rc > 0)
	    printf("%d\n", buf);
	else if (rc < 0){
	    perror("read");
	    sleep(1);
	}
    }
    cout << "f1 finished.\n";
    pthread_exit((int*)8);
}

int main(){
    cout << "Program1 started.\n";
    pthread_t id;
    int e;
    str ff;
    ff.fl = 0;

    if (mkfifo("/tmp/pipe", 0644) != 0)
	perror("mkfifo");
    else
	cout << "/tmp/pipe is created.\n";

    if ((ff.fd = open("/tmp/pipe", O_RDONLY | O_NONBLOCK)) <= 0)
	perror("open");
    else
	cout << "/tmp/pipe is opened.\n" ;

    pthread_create(&id, NULL, f1, &ff);
    cout << "Press key.\n";
    getchar();
    ff.fl = 1;
    pthread_join(id, (void**)&e);
    cout << "Completion code: " << e << endl;
    close(ff.fd);
    unlink("/tmp/pipe");
    cout << "Program1 finished.\n";
    return 0;
}
