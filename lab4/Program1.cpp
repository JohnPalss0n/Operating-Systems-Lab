#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char *argv[]){
    cout << "\nProgram1 started.\n";
    cout << getpid() << endl;
    cout << getppid() << endl;
    cout << argc << "\n";
    for (int i = 0; i < argc; ++i){
	cout << argv[i] << endl;
	//cout << argv[i] << ' ' << flush;	
	if (i == argc - 1)
	    cout << endl;
	else
	    sleep(2);
    }
    cout << "Program1 finished.\n";
    exit(9);
}
