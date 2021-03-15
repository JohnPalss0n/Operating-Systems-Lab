#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main(){
    cout << "Program2 started.\n";
    int pid, status;
    if ((pid = fork()) < 0)
	perror("fork");
    else if (!pid){
	cout << getppid() << endl;
	execl("./Program1", "./Program1", "7", "10", "324", NULL);
	perror("execl");
    }else{
	cout << pid << endl;
	cout << getppid() << endl;
	while (waitpid(pid, &status, WNOHANG) == 0){
	    cout << "Wait...\n";
	    usleep(500000);
	}
	cout << "Completion code: " << WEXITSTATUS(status) << endl;
    }
    cout << "Program2 finished.\n";	
    return 0;
}
