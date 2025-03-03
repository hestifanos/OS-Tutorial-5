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
int total_bellcurve_grade = 0;
pthread_mutex_t lock;
pthread_mutex_t file_lock;

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

void *save_bellcurve(void *grade) {
    pthread_mutex_lock(&file_lock);
    FILE *file = fopen("bellcurve.txt", "a");
    if (file == NULL) {
        printf("Error writing on the file\n");
        pthread_mutex_unlock(&file_lock);
        pthread_exit(0);
    }

    pthread_mutex_lock(&lock);
    int grade_value = *((int *)grade);
    int curved_grade = grade_value * 1.5;
    total_bellcurve_grade += curved_grade;
    total_grade += grade_value;
    fprintf(file, "%d\n", curved_grade);
    pthread_mutex_unlock(&lock);

    fclose(file);
    pthread_mutex_unlock(&file_lock);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    pthread_t thread1, threads[10];
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&file_lock, NULL);
    my_barrier_init(&barrier, 2);

    pthread_create(&thread1, NULL, read_grades, (void *)argv[1]);
    my_barrier_wait(&barrier);

    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, save_bellcurve, &grades[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total grade: %d\n", total_grade);
    printf("Average before curve: %d\n", total_grade / 10);
    printf("Average after curve: %d\n", total_bellcurve_grade / 10);

    pthread_join(thread1, NULL);
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&file_lock);
    my_barrier_destroy(&barrier);

    return 0;
}