#include <stdlib.h>
#include <stdio.h>

#include "pthread.h"

#define NTHREADS 7
#define APP_DEBUG

void* SayHello(void* arg) {
    pthread_acquire();
    printf("Hello, World %d\n", (int)arg);
    pthread_release();
    return NULL;
}

int application_entry(int argc, char** argv) {
    unsigned int i = 0;
    pthread_t threads[NTHREADS];
    for (i = 0; i < NTHREADS; i++) {
#ifdef APP_DEBUG
        pthread_acquire();
        printf("app) Spawning thread. id=%d\n", i);
        pthread_release();
#endif
        pthread_create(&(threads[i]), NULL, SayHello, (void*)(i));
    }
    for (i = 0; i < NTHREADS; i++) {
#ifdef APP_DEBUG
        pthread_acquire();
        printf("app) Joining thread. id=%d\n", i);
        pthread_release();
#endif
        pthread_join(threads[i], NULL);
    }
    return 0;
}
