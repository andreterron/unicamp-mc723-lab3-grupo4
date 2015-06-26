#include "pthread.h"

#include "uos.h"

#define PTHREAD_DEBUG
#ifdef PTHREAD_DEBUG
#include <stdio.h>
#endif
#include <string.h>


#define ERROR_OUT_OF_THREADS             -1
#define ERROR_PROCESSOR_BUSY             -2

volatile int* gLockPool = NULL;
volatile static int __pthread_initialized = 0;

typedef enum _thread_state
{
    IDLE = 0,
    INITIALIZING,
    STOPPED,
    RUNNING,
    FINISHED
} thread_state_t;

typedef struct _thread
{
    uos_thread_t    os_thread;
    thread_state_t  state;
    void*           (*func)(void*);
    void*           arg;
    void*           retval;
} thread_t;

thread_t __thread_pool[MAX_THREADS];

int __try_acquire_lock(volatile int* lock)
{
    return *lock;
}

void __acquire_lock(volatile int* lock)
{
    while (__try_acquire_lock(lock));
}

void __release_lock(volatile int* lock)
{
    *lock = 0;
}

void __thread_call(void* thread)
{
    thread_t* thr = &(__thread_pool[(int)thread]);

    thr->state = RUNNING;
    if ( thr->func != NULL ) {
        thr->func(thr->arg);
    }
    thr->func = NULL;
    thr->state = FINISHED;
}

void pthread_acquire() {
    __acquire_lock(gLockPool);
}
void pthread_release() {
    __release_lock(gLockPool);
}

int pthread_create(
    pthread_t* thread,
    const pthread_attr_t* attributes,
    void* (*start_routine)(void*),
    void* arg
) {
#ifdef PTHREAD_DEBUG
    pthread_acquire();
    printf("ptr) Creating new thread.\n");
    pthread_release();
#endif
    if (!__pthread_initialized) {
        uos_acquire_processor_lock();
#ifdef PTHREAD_DEBUG
        printf("ptr) Initializing pthreads library.\n");
#endif
        if (!__pthread_initialized) {
            gLockPool = uos_get_base_lock_address();
            memset(__thread_pool, 0, MAX_THREADS * sizeof(__thread_pool[0]));
            __pthread_initialized = 1;
        }
        uos_release_processor_lock();
    }
    unsigned int t;
    unsigned int p;
    for ( t = 0; t < MAX_THREADS; t++ ) {
        __acquire_lock(gLockPool);
        if ( __thread_pool[t].state == IDLE ) {
            __thread_pool[t].state = INITIALIZING;
            __thread_pool[t].os_thread.func = __thread_call;
            __thread_pool[t].os_thread.arg = (void*)t;
#ifdef PTHREAD_DEBUG
            printf("ptr) Thread allocated to tid=%d\n",t);
#endif
            __release_lock(gLockPool);
            break;
        }
        __release_lock(gLockPool);
    }
    if ( t == MAX_THREADS ) {
        return ERROR_OUT_OF_THREADS;
    } else {
#ifdef PTHREAD_DEBUG
        pthread_acquire();
        printf("ptr) Initializing thread descriptor for tid=%d\n",t);
        pthread_release();
#endif
        __thread_pool[t].func = start_routine;
        __thread_pool[t].arg = arg;
        __thread_pool[t].state = STOPPED;
    }

#ifdef PTHREAD_DEBUG
    pthread_acquire();
    printf("ptr) Allocating tid=%d to a processor.\n",t);
    pthread_release();
#endif
    int err = 0;
    do {
        err = uos_start_processor((int)&(__thread_pool[t].os_thread));
    } while ( err == -1 );
#ifdef PTHREAD_DEBUG
    pthread_acquire();
    printf("ptr) Thread tid=%d allocated.\n",t);
    pthread_release();
#endif
    *thread = t;

    return 0;
}

void pthread_join(pthread_t thread, void** retval)
{
    thread_state_t state;
    do {
        state = __thread_pool[thread].state;
    } while ( state != FINISHED );
    __acquire_lock(gLockPool);
    if (retval) {
        *retval = __thread_pool[thread].retval;
    }
    __release_lock(gLockPool);
}
