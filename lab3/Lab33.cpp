#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
using namespace std;

struct ff{
    int* fd;
    bool fl;
};

void* f1(void* arg){
    cout << "f1 started.\n";
    int rc, buf = 0;
    while (((ff*)arg)->fl == 0){
	buf++;
	rc = write(((ff*)arg)->fd[1], &buf, sizeof(buf));
        sleep(1);
    }
    cout << "f1 end.\n";
    pthread_exit((int*)8);
}

void* f2(void* arg){
    cout << "f2 started.\n";
    int rc = 0, buf;
    while (((ff*)arg)->fl == 0){
	rc = read(((ff*)arg)->fd[0], &buf, sizeof(buf));
	if (rc > 0){
	    printf("%d\n", buf);
	}
	else if (rc == -1){
	    perror("read");
	    sleep(1);
	}
    }
    cout << "f2 end.\n";
    pthread_exit((int*)9);
}

int main(){
    cout << "Program started.\n";
    pthread_t id1, id2;
    bool fl1 = 0, fl2 = 0;
    int e1, e2;
    int filedes[2]; //0 - для чтения, 1 - для записи
    ff ff1, ff2;
    ff1.fl = fl1;
    ff2.fl = fl2;
    ff1.fd = filedes;
    ff2.fd = filedes;
    if (pipe(filedes) != 0)
	cout << "Error creating pipe.";
    int status = fcntl(filedes[0], F_GETFL);
    fcntl(filedes[0], F_SETFL, O_NONBLOCK | status);
    if (pthread_create(&id1, NULL, f1, &ff1) != 0)
	cout << "Error creating thread f1.";
    if (pthread_create(&id2, NULL, f2, &ff2) != 0)
	cout << "Error creating thread f2.";
    cout << "Press key.\n";
    getchar();
    ff1.fl = 1;
    ff2.fl = 1;
    pthread_join(id1, (void**)&e1);
    pthread_join(id2, (void**)&e2);
    cout << "Completion codes: " << e1 << ", " << e2 << endl;
    close(filedes[0]);
    cout << "filedes[0] closed.\n";
    close(filedes[1]);
    cout << "filedes[1] closed.\n";
    return 0;
}
