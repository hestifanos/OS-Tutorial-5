#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct thread_args {
    long id;
    unsigned int delay;
    const char *message;
};

static void *print_message(void *argument) {
    const struct thread_args *args = argument;
    sleep(args->delay);
    printf("%s! It's me, thread %ld!\n", args->message, args->id);
    return NULL;
}

int main(void) {
    pthread_t threads[2];
    struct thread_args args[] = {
        {1, 1, "Hello World"},
        {2, 2, "Goodbye World"}
    };
    size_t created = 0;

    for (; created < 2; created++) {
        int error = pthread_create(&threads[created], NULL, print_message, &args[created]);
        if (error != 0) {
            fprintf(stderr, "Could not create thread: %s\n", strerror(error));
            break;
        }
    }

    int result = created == 2 ? EXIT_SUCCESS : EXIT_FAILURE;
    while (created > 0) {
        int error = pthread_join(threads[--created], NULL);
        if (error != 0) {
            fprintf(stderr, "Could not join thread: %s\n", strerror(error));
            result = EXIT_FAILURE;
        }
    }
    return result;
}
