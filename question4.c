#define _XOPEN_SOURCE 600 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int total_grade = 0;
pthread_mutex_t lock;


//syncronize the threads using mutex
void *class_totol(void *grade) {
    int *cur_grade = (int *)grade;
    pthread_mutex_lock(&lock); //lock the mutex
    total_grade += *cur_grade;
    pthread_mutex_unlock(&lock); //unlock the mutex
    pthread_exit(0);
}

//use mutex to synchronize the threads
int main() {
    pthread_t thread[5];
    int grades[5];
    int i;
  

    pthread_mutex_init(&lock, NULL); // initialize the mutex

    for (i = 0; i < 5; i++) {
        printf("Enter grade for student %d: ", i + 1);
        scanf("%d", &grades[i]);
    }

    for(i = 0; i < 5; i++) {
        pthread_create(&thread[i], NULL, class_totol, (void *)&grades[i]);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("Total grade: %d\n", total_grade);

   pthread_mutex_destroy(&lock); //destroy the mutex
}