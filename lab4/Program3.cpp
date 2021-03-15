#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

#define STACK_SIZE (1024 * 1024)

static int childFunc(void *arg){
    cout << getppid() << endl;
    cout << getpid() << endl;
    execl("./Program1", "./Program1", "7", "10", "324", NULL);
    perror("execl");
}

int main(){
    cout << "Program3 started.\n";
    int pid, status;
    char *stack;
    char *stackTop;
    stack = (char*)malloc(STACK_SIZE);
    pid = clone(childFunc, stackTop, SIGCHLD, NULL);
    if (pid == -1)
	perror("clone");
    while (waitpid(pid, &status, WNOHANG) == 0){
	cout << "Wait...\n";
	usleep(500000);
    }
    cout << "Completion code: " << WEXITSTATUS(status) << endl;
    delete[] stack;
    cout << "Program3 finished.\n";	
    return 0;
}
