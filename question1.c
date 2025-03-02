#define _XOPEN_SOURCE 600 // Required for barriers to work MEANS THAT WE ARE USING POSIX BARRIERS. barriers are used to synchronize threads
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *hello_world(void *tid) {
    // random sleep time
    int sleep_time = rand() % 5; // this is used to generate a random number between 0 and 5
    sleep(sleep_time);
    printf("Hello World! It's me, thread %ld!\n", (long)tid);
    pthread_exit(NULL);
}

void *goodbye_world(void *tid) {
    // random sleep time
    int sleep_time = rand() % 5;
    sleep(sleep_time);
    printf("Goodbye World! It's me, thread %ld!\n", (long)tid);
    pthread_exit(NULL);
}


int main(){

    pthread_t thread1, thread2;
    long t1 = 1, t2 = 2;
    pthread_create(&thread1, NULL, hello_world, (void *)t1);
    pthread_create(&thread2, NULL, goodbye_world, (void *)t2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;

}