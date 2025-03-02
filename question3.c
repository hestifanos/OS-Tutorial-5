#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// prompt the instructor for student name, id and grade and create a struct named student containing these three fields, finally print 3 of them
struct student {
    char name[50];
    int id;
    int grade;
};

// print the 3 fields of the student struct with grade be the cureved grade
void *bellcurve(void *student) {
    struct student *s = (struct student *)student;
    int curved_grade = s->grade * 1.5;
    printf("Name: %s, ID: %d, Curved Grade: %d\n", s->name, s->id, curved_grade);
    pthread_exit(0);
}

int main() {
    pthread_t threads[5];
    struct student students[5];
    int i;

    for (i = 0; i < 5; i++) {
        printf("Enter name for student %d: ", i + 1);
        scanf("%s", students[i].name);
        printf("Enter ID for student %d: ", i + 1);
        scanf("%d", &students[i].id);
        printf("Enter grade for student %d: ", i + 1);
        scanf("%d", &students[i].grade);
    }

    for(i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, bellcurve, (void *)&students[i]);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}



