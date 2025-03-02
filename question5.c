#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int trip_count;
} my_barrier_t;

int my_barrier_init(my_barrier_t *barrier, unsigned int count) {
    barrier->trip_count = count;
    barrier->count = 0;
    if (pthread_mutex_init(&barrier->mutex, NULL) != 0) {
        return -1;
    }
    if (pthread_cond_init(&barrier->cond, NULL) != 0) {
        pthread_mutex_destroy(&barrier->mutex);
        return -1;
    }
    return 0;
}

int my_barrier_wait(my_barrier_t *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    if (barrier->count >= barrier->trip_count) {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
    } else {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}

int my_barrier_destroy(my_barrier_t *barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
    return 0;
}

my_barrier_t barrier;
int total_grade = 0;
int grades[10];
pthread_mutex_t lock;

void *read_grades(void *filename) {
    FILE *file = fopen((char *) filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        pthread_exit(0);
    }
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 10; i++) {
        fscanf(file, "%d", &grades[i]);
    }
    pthread_mutex_unlock(&lock);
    fclose(file);
    my_barrier_wait(&barrier);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    pthread_t thread;
    pthread_mutex_init(&lock, NULL);
    my_barrier_init(&barrier, 2);

    pthread_create(&thread, NULL, read_grades, (void *)argv[1]);

    my_barrier_wait(&barrier);

    // Process grades after the barrier
    for (int i = 0; i < 10; i++) {
        total_grade += grades[i];
    }
    printf("Total grade: %d\n", total_grade);

    pthread_join(thread, NULL);
    pthread_mutex_destroy(&lock);
    my_barrier_destroy(&barrier);

    return 0;
}