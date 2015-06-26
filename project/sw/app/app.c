#include <stdlib.h>
#include <stdio.h>

#include "pthread.h"

#define NTHREADS 4

void* SayHello(void* arg) {
    pthread_acquire();

    pthread_release();
    return NULL;
}

int application_entry(int argc, char** argv) {
    unsigned int i = 0;
    pthread_t threads[NTHREADS];
    for (i = 0; i < NTHREADS; i++) {
        pthread_create(&(threads[i]), NULL, SayHello, (void*)(i));
    }
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[4], NULL);
    }
    return 0;
}
