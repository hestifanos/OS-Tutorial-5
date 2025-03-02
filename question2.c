#define _XOPEN_SOURCE 600 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Create 5 threads for 5 students provided by the instructor when promted to enter the grades
// each threas uses a function called bellcurve whic take grade and curve it by multiplying it by 1.5 and then print the curved grade
void *bellcurve(void *grade) {
    int curved_grade = *((int *)grade) * 1.5;
    printf("Curved Grade: %d\n", curved_grade);
    pthread_exit(0);
}

int main() {
    pthread_t threads[5];
    int grades[5];
    int i;

    for (i = 0; i < 5; i++) {
        printf("Enter grade for student %d: ", i + 1);
        scanf("%d", &grades[i]);
        //pthread_create(&threads[i], NULL, bellcurve, (void *)&grades[i]);
    }

    for(i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, bellcurve, (void *)&grades[i]);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

