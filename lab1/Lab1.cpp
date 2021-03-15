#include <pthread.h>
#include <unistd.h>
#include <iostream>
using namespace std;

void* f1(void* arg){
    cout << "f1 started.\n";
    while (*(bool*)arg == 0){
        cout << "1\n";
        sleep(1);
    }
    cout << "f1 end.\n";
    pthread_exit((int*)8);
}

void* f2(void* arg){
    cout << "f2 started.\n";
    while (*(bool*)arg == 0){
        cout << "2\n";
        sleep(1);
    }
    cout << "f2 end.\n";
    pthread_exit((int*)9);
}

int main(){
    cout << "Program started.\n";
    pthread_t id1, id2;
    bool fl1 = 0, fl2 = 0;
    int e1, e2;
    pthread_create(&id1, NULL, f1, &fl1);
    pthread_create(&id2, NULL, f2, &fl2);
    cout << "Press key.\n";
    getchar();
    fl1 = 1;
    fl2 = 1;
    pthread_join(id1, (void**)&e1);
    pthread_join(id2, (void**)&e2);
    cout << "Completion codes: " << e1 << ", " << e2 << endl;
    return 0;
}
