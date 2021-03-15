#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>
using namespace std;

struct fm{
    bool fl;
    pthread_mutex_t* mutex;
    timespec tp;
};

void* f1(void* arg){
    cout << "f1 started.\n";
    int rc;
    while (((fm*)arg)->fl == 0){
	clock_gettime(CLOCK_REALTIME, &((fm*)arg)->tp);
	((fm*)arg)->tp.tv_sec += 2;
	rc = pthread_mutex_timedlock(((fm*)arg)->mutex, &((fm*)arg)->tp);
	if (rc != 0){
 	    cout << strerror(rc) << endl;
	    //sleep(1);
	    continue;
	}
	for (int i = 0; i < 5; ++i){
	   cout << "1\n";
	   sleep(1);
	}
	pthread_mutex_unlock(((fm*)arg)->mutex);
	sleep(1);
    }
    cout << "f1 end.\n";
    pthread_exit((int*)8);
}

void* f2(void* arg){
    cout << "f2 started.\n";
    int rc;
    while (((fm*)arg)->fl == 0){
	clock_gettime(CLOCK_REALTIME, &((fm*)arg)->tp);
	((fm*)arg)->tp.tv_sec += 2;
	rc = pthread_mutex_timedlock(((fm*)arg)->mutex, &((fm*)arg)->tp);
	if (rc != 0){
 	    cout << strerror(rc) << endl;
	    continue;
	}
	for (int i = 0; i < 5; ++i){
	   cout << "2\n";
	   sleep(1);
	}
	pthread_mutex_unlock(((fm*)arg)->mutex);
	sleep(1);
    }
    cout << "f2 end.\n";
    pthread_exit((int*)9);
}

int main(){
    cout << "Program started.\n";
    pthread_t id1, id2;
    pthread_mutex_t m;
    bool fl1 = 0, fl2 = 0;
    int e1, e2;
    pthread_mutex_init(&m, NULL);
    fm fm1, fm2;
    fm1.fl = fl1;
    fm2.fl = fl2;
    fm1.mutex = &m;
    fm2.mutex = &m;
    pthread_create(&id1, NULL, f1, &fm1);
    pthread_create(&id2, NULL, f2, &fm2);
    cout << "Press key\n";
    getchar();
    fm1.fl = 1;
    fm2.fl = 1;
    pthread_join(id1, (void**)&e1);
    pthread_join(id2, (void**)&e2);
    cout << "Completion codes: " << e1 << ", " << e2 << endl;
    pthread_mutex_destroy(&m);
    return 0;
}
