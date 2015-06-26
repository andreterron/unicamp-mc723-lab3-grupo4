#ifndef _PTHREAD_H_
#define _PTHREAD_H_

#define MAX_THREADS 256
#define MAX_LOCKS 2048

typedef unsigned long pthread_t;
typedef unsigned long pthread_attr_t;

void pthread_acquire();
void pthread_release();

int pthread_create(
    pthread_t* thread,
    const pthread_attr_t* attributes,
    void* (*start_routine)(void*),
    void* arg
);

void pthread_join(
    pthread_t thread,
    void** retval
);

#endif //_PTHREAD_H_

